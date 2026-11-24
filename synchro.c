#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("Child: Working...\n");
        sleep(3);
        printf("Child: Finished\n");
        exit(10);
    }

    int status;
    wait(&status);

    if (WIFEXITED(status)) {
        printf("Parent: Child exit code = %d\n",
               WEXITSTATUS(status));
    }

    return 0;
}