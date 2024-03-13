

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <errno.h>
#include <assert.h>


#ifndef IPV6_DSTOPTS
#define IPV6_DSTOPTS IPV6_2292DSTOPTS
#endif

#define RESPONSE_BUFLEN 2048


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

char* get_hex_dump(const char* src, int len){
    /* Creating Hex Dump*/
    int i;
    char* hex_dump = (char*)malloc((len*3+1)*sizeof(char));
    
    
    for(i=0; i<len; i++){
        // ngx_sprintf wont work
        sprintf(hex_dump + i*3, "%02X", (unsigned char)src[i]);
        //ngx_log_stderr(0, "[CUSTOM LOG] i=%ui, out : %02X, out2 : %02x, out3 : %x", 
        //i, opts[i], opts[i], opts[i]);
        hex_dump[i*3+2] = ' ';
    }
    hex_dump[(len-1)*3+2] = '\n';
    hex_dump[len*3] = '\0';
    return hex_dump;
}

void printf_str_with_null(const char* str, const uint32_t len){
    int i=0;
    for(; i<len; i++){
        if(str[i] != '\0'){
            printf("%c", str[i]);   
        }
        else{
            printf("*");
        }
    }
}


// Huge memory leak. Use cautiously
char* str_replace_nulls(const char* str, const uint32_t len, const char null_replacement){
    char* str_result = (char*) malloc(sizeof(char)*(len+1));
    int i=0;
    for(; i<len; i++){
        str_result[i] = (str[i] == '\0' ? null_replacement : str[i]) ;
    }
    str_result[len] = '\0';
    return str_result;
}

void print_in6_addr(struct in6_addr* addr){
    char* hex_dump = get_hex_dump((char*)addr, sizeof(struct in6_addr));
    printf("IPv6 Address - %s\n", hex_dump);
    free(hex_dump);
}

int echo_server(int portno)
{
    char resp[] = "HTTP/1.0 200 OK\r\n"
                  "Server: webserver-c\r\n"
                  "Content-type: text/html\r\n\r\n"
                  "<html>hello, world</html>\r\n";

    char client_addr_str[INET6_ADDRSTRLEN];

    int sockfd, clientfd;
    socklen_t socklen;
    char buffer[RESPONSE_BUFLEN];
    struct sockaddr_in6 serv_addr, cli_addr;
    struct hostent *hostaddr;
    int i, n=0, addr;
    int reuseaddr = 1;
    int rc = 0;
    int buflen = 0;

    printf("[echo_server] Starting on port %d\n", portno);

    // create socket
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("Server ERROR opening socket");


    // set the server address
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port = htons(portno);
    serv_addr.sin6_addr = (struct in6_addr)IN6ADDR_ANY_INIT ;

    //Bind
    if (bind(sockfd,(struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0)
        error("Server ERROR bind failed");
    
    printf("[echo_server] Bind done\n");
     
    //Listen
    listen(sockfd , 10);
     
    //Accept and incoming connection

    printf("[echo_server] Waiting for incoming connections\n");
     
    socklen_t slen = sizeof(struct sockaddr_in6);
     
    //accept connection from an incoming client
    while(1){
        clientfd = accept(sockfd, (struct sockaddr *)&cli_addr, &slen);
        if (clientfd < 0)
            error("Server ERROR accept failed");

        inet_ntop(AF_INET6, (void*) (&cli_addr.sin6_addr), client_addr_str, INET6_ADDRSTRLEN);
        printf("[echo_server] Got one connection, from client \'%s\'\n", client_addr_str);





        bzero(buffer,RESPONSE_BUFLEN);
        rc = getsockopt(clientfd, IPPROTO_IPV6, IPV6_DSTOPTS, buffer, &socklen);
        if (rc == 0 && socklen != 0) {
            char *p = (char *)buffer;
            printf("[echo_server] opts with len=%d\n", socklen);
            for (i=0; i<socklen; i++) printf("%02X ", (unsigned char)p[i]);
            printf("\n");
        }
        else
        {
            printf("[echo_server] getsockopts failed rc = %d, socklen = %d, errno = %d\n", rc, socklen, errno);
        }

        // Receive response from client
        bzero(buffer, RESPONSE_BUFLEN);
        n = recv(clientfd, buffer, RESPONSE_BUFLEN, 0);
        if (n < 0) error("[echo_server] ERROR reading from socket");
        
        buflen = strlen(buffer);
        printf("[echo_server] Received Request(n=%d, strlen(buffer)=%d): \n-----------\n%s\n-----------\n", n, buflen, str_replace_nulls(buffer, n, '*'));

        // Send HTTP msg back to client
        n = send(clientfd, resp, strlen(resp), 0);
        if (n < 0) error("[echo_server] Server ERROR writing to socket");

        close(clientfd);
    
    }

    // Code never reaches here
    close(sockfd);
    printf("[echo_server] exiting.\n");
    
    return 0; 
}

void print_usage(const char* prog_name){
    printf("IP Opts sever\n");
    printf("Usage: %s [port]\n", prog_name);
}

int main(int argc, char *argv[])
{
    int portno = 0;
    int rc = 0;
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }
    portno = atoi(argv[1]);
    // atoi returns the converted value or 0 on error.
    if(portno <= 0 || portno >= 60000){
        // either wrong format or wrong value
        print_usage(argv[0]);
        printf("Invalid port %s\n", argv[1]);
        return 1;
    }
    
    rc = echo_server(portno);

    if(rc != 0){
        // Todo : Do we need this ?
        return rc;
    }

    return 0; 
}