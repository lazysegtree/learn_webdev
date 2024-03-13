#ifndef UTIL_GEN_H
#define UTIL_GEN_H

#include "common.h"

typedef struct thread_data_s{
    // Input
    int idx;
    args_t* args;

    // Output
    int cnt_success;
    int cnt_fail;
    int client_port;

    int debug;
    int verbose;

}thread_data;

int generate_request(args_t* args);

#define thread_to_print(start_time) (((time(NULL) - start_time) % 8) == 0) // seconds

#endif