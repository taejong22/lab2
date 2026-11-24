#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];
    char buffer[100];

    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        close(fd[0]);

        const char* msg = "Hello from child!";
        write(fd[1], msg, strlen(msg) + 1);

        close(fd[1]);
        return 0;
    }

    close(fd[1]);

    read(fd[0], buffer, sizeof(buffer));
    printf("Parent received: %s\n", buffer);

    close(fd[0]);
    wait(NULL);

    return 0;
}