#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#ifndef IPV6_DSTOPTS
#define IPV6_DSTOPTS IPV6_2292DSTOPTS
#endif

char* get_hex_dump(const char* src, int len){
    /* Creating Hex Dump*/
    int i;
    char* hex_dump = (char*)malloc((len*3+1)*sizeof(char));
    
    
    for(i=0; i<len; i++){
        // ngx_sprintf wont work
        sprintf(hex_dump + i*3, "%02X", (u_char)src[i]);
        //ngx_log_stderr(0, "[CUSTOM LOG] i=%ui, out : %02X, out2 : %02x, out3 : %x", 
        //i, opts[i], opts[i], opts[i]);
        hex_dump[i*3+2] = ' ';
    }
    hex_dump[len*3] = '\0';
    return hex_dump;
}

char* get_bin_dump(const char* src, int len){
    /* Creating Hex Dump*/
    int i,b;
    char* bin_dump = (char*)malloc((len*9+1)*sizeof(char));
    
    for(i=0; i<len; i++){
        for(b=0; b<8; b++){
            if( (src[i] & (1<<b) ) ){
                bin_dump[i*9+b] = '1';
                bin_dump[i*9+(7-b)] = '1';
            }
            else{
                bin_dump[i*9+b] = '0';
                bin_dump[i*9+(7-b)] = '0';
            }
        }
        bin_dump[i*9+8] = '|';
    }
    bin_dump[len*9] = '\0';
    
    return bin_dump;
}

void set_dummy_ip_opts(char* buffer, int len){
    memset(buffer, 0, len);
    buffer[0] = 0x07 ;
    buffer[1] = 0x20 ;
    buffer[2] = 0x20 ;
    buffer[3] = 0x00 ;
    buffer[4] = 0x01 ;
    buffer[5] = 0x02 ;
    buffer[6] = 0xa1 ;
    buffer[7] = 0xa2 ;
    buffer[8] = 0xa3 ;
}


void set_dummy_ip_opts2(char* buffer, int len){

}

int main(int argc, char* argv[]) {

    if(argc != 4){
        printf("Usage \'program <server> <port> <IP_Opts 0(no) 1(type a) 2(type b)>\'\n");
        printf("Example \'./op4 ::1 8090 0\'");
        printf("Example \'./op4 ::1 8090 1\'");
        return -1;
    }

    const char* server_addr = argv[1];
    int server_port = atoi(argv[2]);
    int ip_opts = atoi(argv[3]);

    // Create an IPv6 socket
    int ipv6_socket = socket(AF_INET6, SOCK_STREAM, 0);
    if (ipv6_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set the destination address to ::1 (IPv6 loopback)
    struct sockaddr_in6 dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin6_family = AF_INET6;
    dest_addr.sin6_port = htons(server_port);  // Change the port as needed
    inet_pton(AF_INET6, server_addr, &(dest_addr.sin6_addr));

    if(ip_opts==1){
        // Set IPv6 destination options (change as needed)
        int hop_limit = 64;  // Set the hop limit (TTL)
        int flow_label = 0;  // Set the flow label

        // Set the hop limit on the socket
        if (setsockopt(ipv6_socket, IPPROTO_IPV6, IPV6_UNICAST_HOPS, &hop_limit, sizeof(hop_limit)) == -1) {
            perror("Setting hop limit failed");
            close(ipv6_socket);
            exit(EXIT_FAILURE);
        }
    }
    else if(ip_opts == 2){
        char buffer[1024];

        set_dummy_ip_opts(buffer, 32);

        int len = 32;
        
        printf("IP OPTS type 2. Sending hex_dump : %s\n, binary_dump : %s\n len = %d\n", 
            get_hex_dump(buffer, 32), 
            get_bin_dump(buffer, 32), 
            len);

        int ret = setsockopt(ipv6_socket, IPPROTO_IPV6, IPV6_DSTOPTS, buffer, 32);
        if(ret){
            perror("Setting dest opts failed");
            close(ipv6_socket);
            exit(EXIT_FAILURE);
        }
    }

    // Connect to the server
    if (connect(ipv6_socket, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) == -1) {
        perror("Connection failed");
        close(ipv6_socket);
        exit(EXIT_FAILURE);
    }

    // Send a sample HTTP GET request
    const char* request = "GET / HTTP/1.1\r\nHost: [::1]\r\n\r\n";
    if (send(ipv6_socket, request, strlen(request), 0) == -1) {
        perror("Sending request failed");
        close(ipv6_socket);
        exit(EXIT_FAILURE);
    }

    // Receive and print the response
    char response[1024];
    ssize_t bytes_received = recv(ipv6_socket, response, sizeof(response), 0);
    if (bytes_received == -1) {
        perror("Receiving response failed");
        close(ipv6_socket);
        exit(EXIT_FAILURE);
    }

    printf("Response from server:\n%.*s\n", (int)bytes_received, response);

    // Close the socket
    close(ipv6_socket);

    return 0;
}
