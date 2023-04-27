#include <iostream>
#include <random>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>


int main(int argc, char* argv[]) {

    // Verifica o número de argumentos de entrada
    if (argc < 2) {
        std::cout << "Por favor, passe como parâmetro apenas o número de inteiros que serão enviados." << std::endl;
        return -1;
    }

    // Obtém argumentos de entrada
    int quantity = atoi(argv[1]);

    // Declara variáveis
    int client_fd;
    struct sockaddr_in address;

    // Cria descritor de arquivo do socket
    // AF_INET especifica o protocolo de internet IPv4 como o domínio de comunicação para o soquete
    // SOCK_STREAM fornece um fluxo de bytes confiável, sequencial e full-duplex entre os dois pontos finais (TCP)
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Erro ao criar socket" << std::endl;
        return -1;
    }

    // Define o endereço IP e a porta do servidor.
    memset(&address, '0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
        std::cout << "Endereço inválido" << std::endl;
        return -1;
    }

    // Conecta ao servidor
    if (connect(client_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }

    // Prepara ambiente para gerar números aleatórios
    int number = 1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 100);

    // Gera "quantity" números, sendo o primeiro sempre igual a um
    for (int i = 0; i < quantity; i++) {

        // Envia número ao servidor
        send(client_fd, &number, sizeof(number), 0);
        std::cout << "Número enviado: " << number << std::endl;

        // Recebe resultado do servidor
        char result[200];
        read(client_fd, result, 200);
        result[strlen(result) - 1] = '\0';
        std::cout << "Resultado: " << result << std::endl;

        // Adiciona um valor aleatório entre 1 e 100 ao número anteriror
        number += dist(gen);
    }

    // Envia zero ao servidor
    int final_num = 0;
    send(client_fd, &final_num, sizeof(final_num), 0);
    std::cout << "Sent final number: " << final_num << std::endl;

    return 0;
}
