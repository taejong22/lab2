#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("Child: PID = %d\n", (int)getpid());
        fflush(stdout);

        while (1)
            pause();
    }

    sleep(1);

    printf("Parent: Sending SIGTERM\n");
    fflush(stdout);

    kill(pid, SIGTERM);
    wait(NULL);

    printf("Parent: Child terminated\n");

    return 0;
}