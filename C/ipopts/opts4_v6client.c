#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
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
    dest_addr.sin6_port = htons(8090);  // Change the port as needed
    inet_pton(AF_INET6, "::1", &(dest_addr.sin6_addr));

    // Set IPv6 destination options (change as needed)
    int hop_limit = 64;  // Set the hop limit (TTL)
    int flow_label = 0;  // Set the flow label

    // Set the hop limit on the socket
    if (setsockopt(ipv6_socket, IPPROTO_IPV6, IPV6_UNICAST_HOPS, &hop_limit, sizeof(hop_limit)) == -1) {
        perror("Setting hop limit failed");
        close(ipv6_socket);
        exit(EXIT_FAILURE);
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
