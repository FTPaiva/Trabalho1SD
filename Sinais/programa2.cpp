#include <iostream>
#include <signal.h>
#include <unistd.h>

void handle_signal(int sig) {
    if (sig == SIGKILL) {
        printf("\nRecebi um SIGKILL.");
    } else if (sig == SIGTERM) {
        printf("\nRecebi um SIGTERM.");
    } else if (sig == SIGCONT) {
        printf("\nRecebi un SIGCONT.");
    }
}


int main() {

    pid_t pid = getpid();
    printf("PID: %d", pid);

    signal(SIGKILL, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGCONT, handle_signal);
    printf("Processo esperando receber algum sinal.");
    while (true) {
        sleep(1);
    }
    return 0;
}
