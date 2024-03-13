#ifndef UTIL_COMMON_H
#define UTIL_COMMON_H


typedef struct global_stats_s{
    int time_sec;
    int response_200;
    int response_4xx;
    int response_5xx;
    int connection_error;
    int socket_created;
    int socket_closed;
}global_stats_t ;

extern global_stats_t global_stats ;


int hex_str_to_buffer(const char* hex_str, char* buffer, int max_len);

char* get_hex_dump(const char* src, int len);

#endif