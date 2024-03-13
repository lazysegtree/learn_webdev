#ifndef UTIL_ARGS_H
#define UTIL_ARGS_H

typedef struct args_s{


    int conn_reuse;
    int delay;
    
    char* prog_name;

    char* server_address; // -s
    int server_port; // -p

    int num_thread; // -t
    int req_per_thread; // -r
      // -d


    int ip_freebind;    
    int ip_opts; // -i
    char* ip_opts_hex_str; // -b
    char* ip_opts_data;
    int ip_opts_len;
    int ipv6; // -6
    
    int verbosity; // -v

    int starting_port;
    int random_port;


}args_t;


extern args_t global_args ;

void print_usage(char* prog_name);

int parse_args_custom(int argc, char* argv[], args_t* args);

void print_args(args_t* args);

#endif