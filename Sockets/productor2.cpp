#include <iostream>
#include <random>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: productor <number of integers to send>" << std::endl;
        return -1;
    }
    int quantidade_num = atoi(argv[1]);

    int client_fd;
    struct sockaddr_in address;

    // Create socket file descriptor
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }

    // Set server IP address and port
    memset(&address, '0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }

    
    int number = 1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);
    for (int i = 0; i < quantidade_num; i++) {

        // Send  number to server
        send(client_fd, &number, sizeof(number), 0);
        std::cout << "Sent number: " << number << std::endl;

        // Receive result from server
        char result[100];
        read(client_fd, result, 100);
        std::cout << "Result: " << result << std::endl;

        // Add a delta to the number
        number += dist(gen);
    }

    // Send final 0 to server
    int final_num = 0;
    send(client_fd, &final_num, sizeof(final_num), 0);
    std::cout << "Sent final number: " << final_num << std::endl;

    return 0;
}
