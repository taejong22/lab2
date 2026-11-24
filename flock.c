#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/wait.h>

int main(void) {
    int fd = open("lock.txt", O_CREAT | O_RDWR, 0644);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    flock(fd, LOCK_EX);
    printf("Parent: Lock acquired\n");
    fflush(stdout);

    pid_t pid = fork();

    if (pid == 0) {
        int child_fd = open("lock.txt", O_RDWR);

        printf("Child: Waiting for lock...\n");
        fflush(stdout);

        flock(child_fd, LOCK_EX);

        printf("Child: Lock acquired\n");

        flock(child_fd, LOCK_UN);
        close(child_fd);
        return 0;
    }

    sleep(2);

    flock(fd, LOCK_UN);
    printf("Parent: Lock released\n");

    wait(NULL);
    close(fd);

    return 0;
}