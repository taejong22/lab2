#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

int main(void) {
    int parent_to_child[2];
    int child_to_parent[2];

    char message[BUFFER_SIZE];

    // 양방향 통신을 위한 파이프 2개 생성
    if (pipe(parent_to_child) == -1 ||
        pipe(child_to_parent) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // 자식 프로세스
        close(parent_to_child[1]);
        close(child_to_parent[0]);

        while (1) {
            ssize_t n = read(parent_to_child[0],
                             message, BUFFER_SIZE);

            if (n <= 0)
                break;

            if (strcmp(message, "exit\n") == 0)
                break;

            printf("\nParent: %s", message);
            printf("Child: ");
            fflush(stdout);

            if (fgets(message, BUFFER_SIZE, stdin) == NULL)
                break;

            if (write(child_to_parent[1],
                      message, strlen(message) + 1) == -1)
                break;

            if (strcmp(message, "exit\n") == 0)
                break;
        }

        close(parent_to_child[0]);
        close(child_to_parent[1]);

        return 0;
    }

    // 부모 프로세스
    close(parent_to_child[0]);
    close(child_to_parent[1]);

    printf("===== Pipe Chat =====\n");
    printf("Type 'exit' to quit.\n\n");

    while (1) {
        printf("Parent: ");
        fflush(stdout);

        if (fgets(message, BUFFER_SIZE, stdin) == NULL)
            break;

        if (write(parent_to_child[1],
                  message, strlen(message) + 1) == -1)
            break;

        if (strcmp(message, "exit\n") == 0)
            break;

        ssize_t n = read(child_to_parent[0],
                         message, BUFFER_SIZE);

        if (n <= 0)
            break;

        if (strcmp(message, "exit\n") == 0)
            break;

        printf("\nChild: %s\n", message);
    }

    close(parent_to_child[1]);
    close(child_to_parent[0]);

    waitpid(pid, NULL, 0);

    printf("\nChat ended.\n");

    return 0;
}