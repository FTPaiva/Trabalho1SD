#include <iostream>
#include <signal.h>
#include <unistd.h>
#include<stdio.h>


int main(int argc, char* argv[]) {

    // Verifica o número de argumentos de entrada
    if (argc != 3) {
        printf("Por favor, passe como parâmetro apenas o PID e sinal.");
        return 1;
    }

    // Obtém argumentos de entrada
    int pid = std::stoi(argv[1]);
    int signal = std::stoi(argv[2]);

    // Testa se processo existe e, caso positivo, envia o sinal
    if (kill(pid, 0) == -1) {
        printf("O processo dado nao existe.\n");
        return -1;
    }
    else {
        if (kill(pid, signal) == -1) {
            printf("Ocorreu um erro ao enviar o sinal.\n");
            return -2;
        }
        else {
            printf("Sinal enviado com sucesso.\n");
            return 0;
        }
    }
}