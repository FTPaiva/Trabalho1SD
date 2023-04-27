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

int main(int argc, char* argv[])
{

    if (argc != 2) {
        printf("O numero de argumentos esta diferente de 2.\n");
        return 1;
   }




    int pipefd[2];
    if (pipe(pipefd) == -1) { // Creating a pipe
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    int limit = std::stoi(argv[1]);;
    int rand_num;
    char str_num[10];
    pid_t pid;
    pid = fork();
    
    if (pid == 0) { // PROCESSO FILHO (CONSUMIDOR)
        close(pipefd[1]); // Fecha o write end do pipe
        while (true) {
            char buffer[10];
            int count = read(pipefd[0], buffer, 10); // Reading data from the read end of the pipe
            buffer[count] = '\0'; // Adding a null terminator to the end of the string
            int num = atoi(buffer); // Converting the string to an integer
            
            if (isPrime(num)) {
                std::cout << "Processo filho recebeu o numero " << num << ", primo." << std::endl;
            }
            else {
                std::cout << "Processo filho recebeu o numero " << num << ", nao primo." << std::endl;
            }

             // Printing the received number to the standard output
            if (num == 0)
                break;
        }
            
        close(pipefd[0]); // Fecha o read end do pipe
    }    
    else { // PROCESSO PAI (PRODUTOR)
        close(pipefd[0]); // Fecha o read end do pipe

        int prev = 1; // Inicializa variaveis da logica do loop
        int current = 1;
        srand(time(NULL)); // Inicializa o gerador de numeros aleatorios

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 100);

        for (int i = 0; i < limit; i++) {
            prev = current; // Atualiza N_i-1

            snprintf(str_num, 20, "%d", current); // Converte o numero a ser enviado em uma string de tamanho fixo
            write(pipefd[1], str_num, sizeof(str_num)); // Escreve a string no pipe
            std::cout << "Processo pai gerou o numero " << current << std::endl; // Printa o numero enviado

            int rand_num = dist(gen);//rand() % 100 + 1; // Gera um numero aleatorio entre 1 e 100
            current = rand_num + prev; // Soma delta ao N_i-1
        }

        snprintf(str_num, 20, "%d", 0); // Converte o numero a ser enviado em uma string de tamanho fixo
        write(pipefd[1], str_num, sizeof(str_num)); // Escreve a string no pipe
        std::cout << "Processo pai gerou o numero " << 0 << std::endl; // Printa o numero enviado

        close(pipefd[1]); // Fecha o write end do pipe

    }

    







    return 0;
}