#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <assert.h>

typedef struct ipopts_ {
    unsigned char type;
    unsigned char length;
    unsigned char pointer;
    uint64_t      metadata;
    unsigned int  overlay_ip;
    unsigned int  substrate_ip;
} __attribute__((packed)) ipopts;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
int main(){
    int portno = 8080;
    uint64_t vcn_id = 123; 
    char *ocid  = "sadfasdf";
    int sockfd;
    char buffer[1024];
    char tbuffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    struct hostent *hostaddr;
    int i, n, addr;
    char http_hdr[256];
    struct timeval timeout, tval_before, tval_after, tval_result;

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Client ERROR opening socket");
    }

    timeout.tv_sec = 3;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (uint8_t *) &timeout, sizeof (timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (uint8_t *) &timeout, sizeof (timeout));

    // set IP Options
    bzero(buffer,1024);
    bzero(tbuffer, 1024);
    ipopts *opts = (ipopts *)buffer;
    opts->type = 0x7;
    // test opts->type = 0x5;
    opts->length = 19;
    opts->pointer = 20;

    //uint64_t md = 0xcafefacedeadbeef; //14627404406152085231
    uint64_t md = vcn_id;
    printf("metadata=%llu\n", md);
    uint32_t w1 = (md & 0xffffffff);
    uint32_t w2 = ((md >> 32) & 0xffffffff);
    opts->metadata = ((uint64_t)htonl(w1) <<32) | htonl(w2);
    printf("htonl metadata=%llu\n", opts->metadata);

    uint32_t ipaddr = 0x01020304;
    opts->overlay_ip = htonl(ipaddr);
    setsockopt(sockfd, IPPROTO_IP, IP_OPTIONS, buffer, opts->length);

    char *p = (char *)opts;
    printf("opts with len=%d\n", opts->length);
    for (i=0; i<opts->length; i++) printf("%02X ", (unsigned char)p[i]);
    printf("\n");

    // set the server address
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    hostaddr = gethostbyname("localhost");
    memcpy(&serv_addr.sin_addr, hostaddr->h_addr, hostaddr->h_length);

    // connect to server
    gettimeofday(&tval_before, NULL);

    static uint8_t addr_counter = 0;
    const struct sockaddr_in client_addr =
    {
        .sin_family = AF_INET,
        .sin_port = 0,
        .sin_addr.s_addr = htonl (0x7F000000 + ((++addr_counter % 0x7F) + 1)),
    };
    if (bind (sockfd, (struct sockaddr *) &client_addr, sizeof (client_addr)) == -1)
        error("Client ERROR on bind");

    if (connect(sockfd, (struct sockaddr *) &serv_addr,
                        sizeof(serv_addr)) < 0)
        error("Client ERROR on connect");
    printf("connected...\n");

    // Send HTTP request to server
    sprintf(http_hdr, "GET / HTTP/1.1\r\n"\
                      "Host: localhost:8001\r\n"\
                      "Accept: */*\r\n" \
                      "\r\n");
    printf("Sending Request: %s\n", http_hdr);

    n = send(sockfd, http_hdr, strlen(http_hdr), 0);
    if (n < 0) error("Client ERROR writing to socket");

    // Receive response from server
    printf("Waiting for response...\n");
    bzero(buffer, 1024);
    n = recv(sockfd, buffer, 1024, 0);
    if (n < 0) error("Client ERROR reading from socket");

    gettimeofday(&tval_after, NULL);

    timersub(&tval_after, &tval_before, &tval_result);

    printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

    printf("Recieved Response: %s\n",buffer);

    close(sockfd);

    //printf("Received %s\n %s", buffer, ocid);
    if (n <= 1 || strncmp(buffer, ocid, n)) {
       fprintf(stderr, "\nPPV2 Test FAILED %ld %s %s\n\n",vcn_id, ocid, buffer );
       exit(1);
    }

    printf("\nPPV2 Test passed\n\n");
    //fprintf(stderr, "\nPPV2 Test passed\n\n");
    return 0;

}