// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct ipopts_ {
    unsigned char type;
    unsigned char length;
    unsigned char pointer;
    uint64_t      metadata;
    unsigned int  overlay_ip;
    unsigned int  substrate_ip;
} __attribute__((packed)) ipopts;

ipopts* get_ip_options(char* buffer, int buffer_size){
    bzero(buffer,buffer_size);
    ipopts *opts = (ipopts *)buffer;
    int i=0;
    opts->type = 0x7;
    // test opts->type = 0x5;
    opts->length = 31;
    opts->pointer = 32;

    // 07 1F 20 CA FE FA CE DE AD BE EF 01 02 03 04 01 F2 F3 F4 00 00 00 00 00 00 00 00 00 00 00 00

    uint64_t md = 0xcafefacedeadbeef; //14627404406152085231
    //uint64_t md = vcn_id;
    printf("metadata=%llu\n", md);

    uint32_t w1 = (md & 0xffffffff);
    uint32_t w2 = ((md >> 32) & 0xffffffff);
    
    opts->metadata = ((uint64_t)htonl(w1) <<32) | htonl(w2);
    
    printf("htonl metadata=%llu\n", opts->metadata);

    uint32_t ipaddr = 0x01020304;
    opts->overlay_ip = htonl(ipaddr);

    uint32_t ipaddr2 = 0x01F2F3F4;
    opts->substrate_ip = htonl(ipaddr2);

    return opts;
    
}

int main(int argc, char const *argv[])
{

    if(argc != 4){
        printf("Wrong args\n"
            "Usage : program [HOSTNAME] [HOSTPORT] [IP_OPTS_SEND]\n"
            "IP_OPTS_SEND should be 0 or 1 \n"
            "Example : \'./op2 127.0.0.1 80 0\' \n"
        );
        return 0;
    }

    const char* hostname = argv[1];
    int server_port = atoi(argv[2]);
    int ip_opts_send = atoi(argv[3]);

    printf("Starting program with args Server hostname : %s, port : %d, To send ip options : %d\n",
    hostname, server_port, ip_opts_send);

    char http_hdr[256];
    int sockfd = 0, valread, client_fd;
    
    char buffer[1024] = {0};


    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    printf("[INFO] Created client socket.\n");


    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, hostname, &serv_addr.sin_addr) <= 0)
    {
        printf(
          "\nInvalid address/ Address not supported \n");
        return -1;
    }


    printf("[INFO] Created server address structure.\n");

    if(ip_opts_send){
        ipopts* opts = get_ip_options(buffer, 1024);
        setsockopt(sockfd, IPPROTO_IP, IP_OPTIONS, buffer, opts->length);

        char *p = (char *)opts;
        printf("opts with len=%d\n", opts->length);
        int i=0;
        for (i=0; i<opts->length; i++) printf("%02X ", (unsigned char)p[i]);
        printf("\n");
    }

    printf("[INFO] Initiated connection of client socket with server socket\n");

    if ((client_fd = connect(sockfd, (struct sockaddr *)&serv_addr,
                 sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    sprintf(http_hdr, "GET / HTTP/1.1\r\n"
              "Host: localhost:8001\r\n"
              "Accept: */*\r\n"
              "\r\n");
    printf("[INFO] Sending Request: %s\n", http_hdr);

    int n = send(sockfd, http_hdr, strlen(http_hdr), 0);

    if (n < 0)
    {
        perror("Client ERROR writing to socket");
    }
    // send(sock, hello, strlen(hello), 0);

    valread = read(sockfd, buffer, 1024);
    printf(" Output \"%s\"\n", buffer);

    // closing the connected socket
    close(client_fd);
    return 0;
}
