#include <iostream>
#include <csignal>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


void signalHandler( int signum ) {
   if (signum == 2) { // Handler do SIGINT
      std::cout << "SIGINT recebido. O processo nao sera terminado.\n"; 
   }
   if (signum == 10) { // Handler do SIGUSR1
      std::cout << "SIGUSR1 recebido. O processo nao sera terminado.\n";
   }
   if (signum == 12) { // Handler do SIGUSR2
      std::cout << "SIGUSR2 recebido. Terminando processo.\n";
      exit(signum);
   }
}


int main (int argc, char* argv[]) {

   // Verifica o número de argumentos de entrada
   if (argc != 2) {
        printf("Por favor, passe como parâmetro apenas o tipo de wait.");
        return 1;
   }

   // Obtém argumentos de entrada
   int waitType = std::stoi(argv[1]);

   // Imprime o PID deste processo
   pid_t pid = getpid(); 
   std::cout << "PID: " << pid << "\n";

   // Estipula os signal handlers
   signal(SIGINT, signalHandler);
   signal(SIGUSR1, signalHandler);
   signal(SIGUSR2, signalHandler);

   // Entra em um tipo de espera, dependendo do argumentos de entrada do programa
   if (waitType == 0) {
      std::cout << "Busy wait.\n";
      while(1) {
      }
   }
   else {
      std::cout << "Blocking wait.\n";
      while(1){
         pause();
      }
   }
   return 0;
}