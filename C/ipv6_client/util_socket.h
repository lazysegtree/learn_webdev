#ifndef UTIL_SOCKET_H
#define UTIL_SOCKET_H

#include "common.h"

int set_socket_ip_opts(int socket_fd, const args_t* args);

int setup_client_socket(int *socket_fd_ptr, int client_port, const args_t* args);

int send_request(int socket_fd, const args_t* args);

int close_socket(int socket_fd);

#endif