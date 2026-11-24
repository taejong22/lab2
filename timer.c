#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void alarmHandler(int signo) {
    if (signo == SIGALRM) {
        const char msg[] = "Alarm received!\n";
        write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    }
}

int main(void) {
    struct sigaction sa = {0};

    sa.sa_handler = alarmHandler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    printf("Waiting for 3 seconds...\n");
    fflush(stdout);

    alarm(3);
    pause();

    printf("Program finished.\n");

    return 0;
}