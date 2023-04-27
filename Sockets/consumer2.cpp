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

    // Declara variáveis
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Cria descritor de arquivo do socket
    // AF_INET especifica o protocolo de internet IPv4 como o domínio de comunicação para o soquete
    // SOCK_STREAM fornece um fluxo de bytes confiável, sequencial e full-duplex entre os dois pontos finais (TCP)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        exit(EXIT_FAILURE);
    }

    // Configura o socket
    // SOL_SOCKET é usado para indicar que a opção se aplica ao próprio soquete em vez de a um protocolo específico.
    // SO_REUSEADDR permite que um soquete se ligue a uma porta que ainda está em uso por outro soquete que está no estado TIME_WAIT.
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        exit(EXIT_FAILURE);
    }

    // Associa socket à porta
    // INADDR_ANY faz com que o socket seja ligado a todas as interfaces locais disponível
    // AF_INET indica que o endereço será do tipo IPv4
    // Porta 8080 escolhida arbitrariamente
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        exit(EXIT_FAILURE);
    }

    // Escuta novas conexões
    // Blocking wait
    // Máximo de 5 conexões na fila
    if (listen(server_fd, 5) < 0) {
        exit(EXIT_FAILURE);
    }

    // Aceita conexão do cliente
    // new_socket = descritor do socket do cliente
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        exit(EXIT_FAILURE);
    }

    while(1){

        // Recebe números inteiro do cliente
        int number;
        read(new_socket, &number, sizeof(number));
        std::cout << "Número recebido: " << number << std::endl;

        // Testa se número recebido é um zero
        if (number == 0) {
            return 0;
        }
        
        // Testa se número é primo
        bool prime = isPrime(number);

        // Envia resultado ao cliente
        char result[200];

        if (prime) {
            strcpy(result, "O número é primo...");
        } else {
            strcpy(result, "O num não é primo...");
        }
        result[strlen(result) - 1] = '\0';
        send(new_socket, result, strlen(result), 0);
    }
    return 0;
}
