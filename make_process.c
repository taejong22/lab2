#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("Child PID: %d\n", (int)getpid());
        printf("Parent PID: %d\n", (int)getppid());
    }
    else {
        printf("Parent PID: %d\n", (int)getpid());
        wait(NULL);
    }

    return 0;
}