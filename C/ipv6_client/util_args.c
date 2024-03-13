//#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "util_args.h"
#include "common.h"


args_t global_args ;

void print_usage(char* prog_name){
    printf("Usage: %s [-s server_address] [-p port] [Optional args ... ]\n",
		prog_name);
    printf("Optional args : \n");
    printf("[-6]                    IPv6 or not\n");
    printf("[--no-ip-opts]          Dont send IP Options\n");
    printf("[-t num_threads] \n");
    printf("[-r reqeust_per_thread] Requests Per thread\n");
    printf("[-v verbosity] \n");
    printf("[--starting-port port] \n");
    printf("[--random-port] \n");
    printf("[-c] [--conn-reuse] \n");
    printf("[--ip-freebind] \n");

}

void init_args_defaults(args_t* args){
    args->conn_reuse = 0;
    args->delay = 0;
    
    args->ip_freebind = 0;

    args->ip_opts = 1;
    args->ip_opts_data = NULL;
    args->ip_opts_hex_str = NULL;
    args->ip_opts_len = 0;

    args->ipv6 = 0;

    args->num_thread = 1;
    args->prog_name = NULL;
    args->req_per_thread = 1;

    args->server_address = NULL;

    args->server_port = -1;
    args->verbosity = 3;
    
    args->starting_port = DEF_STARTING_PORT ;
    args->random_port = 0;

}

int validate_args(args_t* args){
    
    if(! args->server_address ){
        printf("-s server_address must be specified\n");
        return STATUS_PARSE_ERROR;
    }

    if(args->server_port <= 0 ){
        printf("Invalid server_port value\n");
        return STATUS_PARSE_ERROR;
    }

    if(args->num_thread <= 0 ){
        printf("Invalid num_thread value\n");
        return STATUS_PARSE_ERROR;
    }

    if(args->req_per_thread <= 0 ){
        printf("Invalid req_per_thread value\n");
        return STATUS_PARSE_ERROR;
    }

    if(args->starting_port <= 10000 ){
        printf("Invalid value of starting port\n");
        return STATUS_PARSE_ERROR;
    }

    return STATUS_OKAY;
}

int parse_args(int argc, char* argv[], args_t* args){
    char* options="i6s:p:t:r:d:b:v:" ;
    int c = 0;
    
    //opterr = 0;

    while( (c = getopt(argc, argv, options)) != -1){
        //printf("c is %c, optarg is %s", c, optarg);
        switch(c){
            case 's':
                args->server_address = optarg;
                break;
            case 'p':
                args->server_port = atoi(optarg);
                break;
            case 't':
                args->num_thread = atoi(optarg);
                break;
            case 'r':
                args->req_per_thread = atoi(optarg);
                break;
            case 'v':
                args->verbosity = atoi(optarg);
                break;
            case 'i':
                args->ip_opts = 1;
                break;
            case '6':
                args->ipv6 = 1;
                break;
            case '?':
                printf("Unknown arg");
                return STATUS_PARSE_ERROR;
            default:
                return STATUS_PARSE_ERROR;
        }
    }

    return STATUS_OKAY;
}


int parse_args_custom(int argc, char* argv[], args_t* args){
    init_args_defaults(args);
    int i = 1;
    while(i<argc){
        if(strcmp(argv[i], "-p") == 0){
            assert(i+1 < argc);
            args->server_port = atoi(argv[i+1]);
            
            i++;
        }
        else if(strcmp(argv[i], "-s") == 0){
            assert(i+1 < argc);
            args->server_address = argv[i+1];
            i++;
        }
        else if(strcmp(argv[i], "--no-ip-opts") == 0){
            args->ip_opts = 0;
        }
        else if(strcmp(argv[i], "-6") == 0){
            args->ipv6 = 1;
        }
        else if(strcmp(argv[i], "-v") == 0){
            assert(i+1 < argc);
            args->verbosity = atoi(argv[i+1]);
            i++;
        }
        else if(strcmp(argv[i], "-r") == 0){
            assert(i+1 < argc);
            args->req_per_thread = atoi(argv[i+1]);
            i++;
        }
        else if(strcmp(argv[i], "-t") == 0){
            assert(i+1 < argc);
            args->num_thread = atoi(argv[i+1]);
            i++;
        }
        else if(strcmp(argv[i], "--starting-port") == 0){
            assert(i+1 < argc);

            printf("starting port %s\n", argv[i+1]);
            args->starting_port = atoi(argv[i+1]);
            i++;
        }
        else if(strcmp(argv[i], "--random-port") == 0){
            args->random_port = 1;
        }
        else if((strcmp(argv[i], "--conn-reuse")    == 0) || 
                (strcmp(argv[i], "-c")              == 0))
        {
            args->conn_reuse = 1;
        }
        else if(strcmp(argv[i], "--ip-freebind") == 0){
            args->ip_freebind = 1;
        }
        else{
            printf("Invalid/Ill-placed token %s while parsing arguement\n", argv[i]);
            return STATUS_PARSE_ERROR;
        }
        i++;
    }
    return validate_args(args);
}

void print_args(args_t* args){
    printf("Args : ");
    printf("conn_reuse : %d", args->conn_reuse);
    if(args->prog_name) printf(" prog_name = %s", args->prog_name);
    if(args->server_address) printf(" server_address = %s", args->server_address); // -s
    printf(" server_port = %d", args->server_port); // -p
    printf(" num_thread = %d", args->num_thread); // -t
    printf(" req_per_thread = %d\n", args->req_per_thread); // -r
    printf("delay = %d", args->delay);  // -d
    printf(" ip_freebind = %d", args->ip_freebind);
    printf(" ipv6 = %d", args->ipv6); // -6
    printf(" ip_opts = %d", args->ip_opts); // -i
    if(args->ip_opts_hex_str) printf(" ip_opts_hex_str = %s", args->ip_opts_hex_str); // -b
    printf(" verbosity = %d\n", args->verbosity); // -v


    printf(" starting_port = %d", args->starting_port);
    printf(" random_port = %d\n", args->random_port);
}