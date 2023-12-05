// Client side C/C++ program to demonstrate Socket  
// programming  
#include <arpa/inet.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <unistd.h>  
#define PORT 8080  
  
int main(int argc, char const* argv[])  
{  

    char http_hdr[256];
       int sock = 0, valread, client_fd;  
       struct sockaddr_in serv_addr;  
       char* hello = "Hello from client";  
       char buffer[1024] = { 0 };  
       if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {  
               printf("\n Socket creation error \n");  
               return -1;  
       }  
  
       serv_addr.sin_family = AF_INET;  
       serv_addr.sin_port = htons(PORT);  
  
       // Convert IPv4 and IPv6 addresses from text to binary  
       // form  
       if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)  
               <= 0) {  
               printf(  
                       "\nInvalid address/ Address not supported \n");  
               return -1;  
       }  
  
       if ((client_fd  
               = connect(sock, (struct sockaddr*)&serv_addr,  
                               sizeof(serv_addr)))  
               < 0) {  
               printf("\nConnection Failed \n");  
               return -1;  
       }  
       sprintf(http_hdr, "GET / HTTP/1.1\r\n"\
                      "Host: localhost:8001\r\n"\
                      "Accept: */*\r\n" \
                      "\r\n");
    printf("Sending Request: %s\n", http_hdr);

        int n = send(sock, http_hdr, strlen(http_hdr), 0);
       
    if (n < 0){ 
        perror("Client ERROR writing to socket");
    }
       //send(sock, hello, strlen(hello), 0);  

       printf("Hello message sent\n");  
       valread = read(sock, buffer, 1024);  
       printf("%s\n", buffer);  
  
       // closing the connected socket  
       close(client_fd);  
       return 0;  
}  
  