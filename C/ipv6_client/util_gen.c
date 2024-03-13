#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#include "common.h"
#include "util_socket.h"
#include "util_gen.h"


void *thread_start(void* arg){
    thread_data* data = (thread_data*) arg;

    unsigned int i = 0;
    int status;
    int socket_fd;

    //time_t start_time = time(NULL);
    time_t last_print_time = 0;
    time_t cur_time = 0;

    for(i=0; i<data->args->req_per_thread; i++){
        status = setup_client_socket(&socket_fd, data->client_port, data->args);
        if(status==0) status = send_request(socket_fd, data->args);
        if(status==0) status = close_socket(socket_fd);

        if(status==0) data->cnt_success ++;
        else{
            data->cnt_fail++;
            if(data->debug){
                printf("Thread %d, client_port %d. Request failed. errno %d\n", 
                    data->idx, data->client_port, errno);
            }
        } 

        //printf("Time is %ld\n", time(NULL));
        

        if(data->args->verbosity >= 1){
            cur_time = time(NULL);
            if( (cur_time & 0b111) == 0 &&
                cur_time != last_print_time)
            {
                last_print_time = cur_time ;
                printf("Thread %d, %d Success, %d Fail\n", data->idx, data->cnt_success, data->cnt_fail);
            }
        }
        

    }

    if(data->args->verbosity >= 1){
        printf("Thread %d\t Done. %d Success, %d failure\n", 
                data->idx, data->cnt_success, data->cnt_fail);
    }

    pthread_exit(NULL);
}

int generate_request(args_t* args){
    
    pthread_t* threads = (pthread_t*) malloc(args->num_thread * sizeof(pthread_t));
    thread_data* data = (thread_data*) malloc(args->num_thread * sizeof(thread_data));
    unsigned int i=0;
    int status;

    int total_success = 0, total_fail = 0;


    for(i=0; i<args->num_thread; i++){
        data[i].idx = i;
        data[i].args = args;
        data[i].cnt_fail = 0;
        data[i].cnt_success = 0;
        data[i].debug = args->verbosity >= 1;
        data[i].verbose = args->verbosity >= 2;
        if(args->random_port)
            data[i].client_port = 0;
        else{
            data[i].client_port = args->starting_port + i ;
        }
        pthread_create(&(threads[i]), NULL, thread_start, (void*)&(data[i]));
    }

    for(i=0; i<args->num_thread; i++){
        status = pthread_join(threads[i], NULL);
        if(status != 0) {
            return STATUS_UNKOWN_ERROR;
        }

        total_success += data[i].cnt_success;
        total_fail += data[i].cnt_fail;
    }
    
    printf("Generator Done with %d threads, %d requests. %d success %d fail.\n",
            args->num_thread, args->num_thread * args->req_per_thread, 
            total_success, total_fail);

    return STATUS_OKAY;
}
