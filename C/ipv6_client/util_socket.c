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

#include "util_socket.h"
#include "util_common.h"
#include "util_ip_opts.h"

const char* def_request_v6 =  "GET / HTTP/1.1\r\n"
                            "Host: [::1]\r\n\r\n";
const char* def_request_v4 = "GET / HTTP/1.1\r\n"
              "Host: localhost\r\n"
              "Accept: */*\r\n"
              "\r\n";

char response[1024];

int create_socket_addr(const char* addr, int port, struct sockaddr** sockaddr_ptr, int ipv6){
    int status = 0;
    if(ipv6){
        struct sockaddr_in6* addr_v6 = (struct sockaddr_in6*) 
            malloc(sizeof(struct sockaddr_in6));

        memset(addr_v6, 0, sizeof(struct sockaddr_in6));

        addr_v6->sin6_family = AF_INET6;
        addr_v6->sin6_port = htons(port);  // Change the port as needed
        status = inet_pton(AF_INET6, addr, &(addr_v6->sin6_addr));

        if(status != 1){
            return STATUS_PARSE_ERROR;
        }

        *sockaddr_ptr = (struct sockaddr*)addr_v6;

    }
    else{
        struct sockaddr_in* addr_v4 = (struct sockaddr_in*) 
            malloc(sizeof(struct sockaddr_in));

        memset(addr_v4, 0, sizeof(struct sockaddr_in));

        addr_v4->sin_family = AF_INET;
        addr_v4->sin_port = htons(port);  // Change the port as needed
        status = inet_pton(AF_INET, addr, &(addr_v4->sin_addr));

        if(status != 1){
            return STATUS_PARSE_ERROR;
        }
        *sockaddr_ptr = (struct sockaddr*)addr_v4;
    }

    return STATUS_OKAY;
}


int set_socket_opts(int socket_fd, const args_t* args){
    char* ip_opts_buffer = NULL;
    int ip_opts_len = 0;
    int status = 0;
    // set basic opts
    const int opt_value = 1;
    status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(int));
    
    if(!args->ip_opts) return STATUS_OKAY;    

    
    if(args->ip_opts_data){
        ip_opts_buffer = args->ip_opts_data;
        ip_opts_len = args->ip_opts_len;
    }
    else{
        ip_opts_buffer = (args->ipv6 ? default_buffer_v6 : default_buffer_v4);
        ip_opts_len = (args->ipv6 ? DEF_OPTLEN_V6 : DEF_OPTLEN_V4) ;
    }

    if(args->verbosity >= 2)
        printf("IP OPTS . Setting buffer : %s\n, len = %d\n", 
            get_hex_dump(ip_opts_buffer, ip_opts_len), ip_opts_len);

    if(args->ipv6){
        status = setsockopt(socket_fd, IPPROTO_IPV6, IPV6_DSTOPTS, ip_opts_buffer, ip_opts_len);
    }
    else{
        status = setsockopt(socket_fd, IPPROTO_IP, IP_OPTIONS, ip_opts_buffer, ip_opts_len);
    }
    if(status){
        printf("errno %d while setting socketopts\n", errno);
        return STATUS_SOCKET_ERROR;
    }

    return STATUS_OKAY;
}

// close the socket_fd if anything wrong happens
// return STATUS_OKAY on success
int setup_client_socket(int *socket_fd_ptr, int client_port, const args_t* args){
    int socket_fd = -1;

    // Todo Memeory Leak fix
    struct sockaddr* dest_addr = NULL;
    struct sockaddr* client_addr = NULL;
    int dest_addr_size = 0;
    int status = 0;
    int return_code = STATUS_OKAY;

    if(return_code == STATUS_OKAY){
        if(args->ipv6){
            socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
        }
        else{
            socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        }

        //
        if (socket_fd < 0) {
            printf("Error while created socket. errno set to %d \n", errno);

            return_code = STATUS_SOCKET_ERROR;
        }
    }

    if(return_code == STATUS_OKAY){
        status = set_socket_opts(socket_fd, args);
        if (status != STATUS_OKAY){
            printf("Error while setting IP Options\n");
            return_code = STATUS_SOCKET_ERROR;
        }
    }
    

    if(args->verbosity >= 3){
        printf("Bind %d\n", client_port);
    }
    if(args->ipv6){
        status = create_socket_addr("::", client_port, &client_addr, args->ipv6);
        assert(status == STATUS_OKAY);
        status = bind(socket_fd, client_addr, sizeof(struct sockaddr_in6));
    }
    else{
        status = create_socket_addr("127.0.0.1", client_port, &client_addr, args->ipv6);
        assert(status == STATUS_OKAY);
        status = bind(socket_fd, client_addr, sizeof(struct sockaddr_in));
    }

    if(status != 0){
        if (args->verbosity >= 1)
            printf("bind() failed on port %d errno = %d socket_fd = %d\n", client_port, errno, socket_fd);
        
        close(socket_fd);
        return STATUS_SOCKET_ERROR;
    }
    

    status = create_socket_addr(args->server_address, args->server_port, &dest_addr, args->ipv6);
    if(status != 0){
        printf(
          "\nInvalid Address according to IP version : \"%s\"\n", args->server_address);
        close(socket_fd);
        return STATUS_PARSE_ERROR;
    }
    if(args->ipv6){
        status = connect(socket_fd, dest_addr, sizeof(struct sockaddr_in6));
    }
    else{
        status = connect(socket_fd, dest_addr, sizeof(struct sockaddr_in));
    }

    free(dest_addr);
    free(client_addr);

    // Connect to the server
    if (status != 0) {
        if(args->verbosity >= 1)
            printf("connect() to server failed. errno = %d\n", errno);
        close(socket_fd);
        return STATUS_SOCKET_ERROR;
    }


    *socket_fd_ptr = socket_fd;
    return STATUS_OKAY;

}



int send_request(int socket_fd, const args_t* args){
    // Send a sample HTTP GET request
    
    int status ;
    if(args->ipv6){
        status = send(socket_fd, def_request_v6, strlen(def_request_v6), 0);
    }
    else{
        status = send(socket_fd, def_request_v4, strlen(def_request_v4), 0);
    }
    
    if(status == -1) {
        printf("Sending request failed");
        close(socket_fd);
        return STATUS_SEND_ERROR;
    }

    // Receive and print the response
    

    ssize_t bytes_received = recv(socket_fd, response, sizeof(response), 0);
    if (bytes_received == -1) {
        printf("Receiving response failed");
        close(socket_fd);
        return STATUS_RECV_ERROR;
    }
    
    if(args->verbosity >= 2)
        printf("Response from server:\n%.*s\n", (int)bytes_received, response);

    return STATUS_OKAY;
}

int close_socket(int socket_fd){
    return close(socket_fd);
}