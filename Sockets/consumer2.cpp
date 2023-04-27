#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i*i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket file descriptor
    // AF_INET specifies the IPv4 Internet protocol as the communication domain for the socket
    // SOCK_STREAM provides a reliable, sequenced, and full-duplex byte stream between the two endpoints (TCP)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    // SOL_SOCKET is used to indicate that the option applies to the socket itself rather than to a specific protocol.
    // SO_REUSEADDR allows a socket to bind to a port that is still in use by another socket that is in the TIME_WAIT state.
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    // INADDR_ANY is a special constant that represents the "any" address.
    // It means that the server socket will be bound to all available network interfaces on the machine
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    // Blocking wait
    // Max of 3 connections that can be queued up to be accepted
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection
    // new_socket = client socket descriptor
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while(1){

        // Receive integer from client
        int number;
        read(new_socket, &number, sizeof(number));
        std::cout << "Received number: " << number << std::endl;

        // Testa se numero recebido eh um zero
        if (number == 0) {
            return 0;
        }
        
        // Check if number is prime
        bool prime = isPrime(number);

        // Send result back to client
        char result[100];
        if (prime) {
            strcpy(result, "The number is prime.");
        } else {
            strcpy(result, "The number is not prime.");
        }
        send(new_socket, result, strlen(result), 0);
    }
    return 0;
}
