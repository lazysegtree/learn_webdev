#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>
#include <errno.h>

#include "common.h"

#include "util_args.h"

#include "util_socket.h"
#include "util_ip_opts.h"
#include "util_common.h"
#include "util_gen.h"


int main(int argc, char * argv[]){
    int i = 0;
    int socket_fd = 0;
    int status = 0;

    if(parse_args_custom(argc, argv, &global_args) != 0){
        printf("Invalid Args. ");
        print_usage(argv[0]);
        return STATUS_PARSE_ERROR;
    }

    if(global_args.verbosity >= 3){
        print_args(&global_args);
    }

    init_default_ip_opts_buffer();

    generate_request(&global_args);

}