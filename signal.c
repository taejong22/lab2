#include <stdio.h>
#include <signal.h>

void signalHandler(int signo) {
    if (signo == SIGUSR1) {
        printf("SIGUSR1 received!\n");
    }
}

int main(void) {
    struct sigaction sa = {0};

    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    printf("Sending SIGUSR1...\n");

    raise(SIGUSR1);

    printf("Signal handled.\n");

    return 0;
}