#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include <random>
#include <arpa/inet.h>


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


int main(int argc, char* argv[]) {

    // Verifica o número de argumentos de entrada
    if (argc != 2) {
        printf("Por favor, passe como parâmetro apenas o número de inteiros que serão enviados.");
        return 1;
   }

    // Cria o pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) { 
        exit(EXIT_FAILURE);
    }

    // Obtém argumentos de entrada
    int limit = std::stoi(argv[1]);;

    // Cria processo filho
    pid_t pid;
    pid = fork();
    
    // PROCESSO FILHO (CONSUMIDOR)
    if (pid == 0) {

        // Fecha o write end do pipe
        close(pipefd[1]); 

        while (true) {

            char buffer[10];
            int count = read(pipefd[0], buffer, 10); // Lendo dados do pipe
            buffer[count] = '\0'; // Adiciona terminador nulo ao fim da string
            int num = atoi(buffer); // Converte string para int
            
            // Printing the received number to the standard output
            if (num == 0)
                break;
                
            if (isPrime(num)) {
                std::cout << "Processo filho recebeu o numero " << num << ", primo." << std::endl;
            }
            else {
                std::cout << "Processo filho recebeu o numero " << num << ", nao primo." << std::endl;
            }

        }
            
        close(pipefd[0]); // Fecha o read end do pipe
    }
    // PROCESSO PAI (PRODUTOR)
    else {

        // Fecha o read end do pipe
        close(pipefd[0]); 

        // Inicializa variaveis da logica do loop
        int prev = 1; 
        int current = 1;

        // Inicializa o gerador de numeros aleatorios
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 100);

        char str_num[10];

        for (int i = 0; i < limit; i++) {
            prev = current; // Atualiza N_i-1

            snprintf(str_num, 20, "%d", current); // Converte o numero a ser enviado em uma string de tamanho fixo
            write(pipefd[1], str_num, sizeof(str_num)); // Escreve a string no pipe
            std::cout << "Processo pai gerou o numero " << current << std::endl; // Printa o numero enviado

            int rand_num = dist(gen); // Gera um numero aleatorio entre 1 e 100
            current = rand_num + prev; // Soma delta ao N_i-1
        }

        snprintf(str_num, 20, "%d", 0); // Converte o numero a ser enviado em uma string de tamanho fixo
        write(pipefd[1], str_num, sizeof(str_num)); // Escreve a string no pipe
        std::cout << "Processo pai gerou o numero " << 0 << std::endl; // Printa o numero enviado

        close(pipefd[1]); // Fecha o write end do pipe

    }
    
    return 0;
}