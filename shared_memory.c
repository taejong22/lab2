#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main(void) {
    int shm_id = shmget(IPC_PRIVATE, 100, IPC_CREAT | 0600);

    if (shm_id == -1) {
        perror("shmget");
        return 1;
    }

    char* shared = shmat(shm_id, NULL, 0);

    if (shared == (void*)-1) {
        perror("shmat");
        shmctl(shm_id, IPC_RMID, NULL);
        return 1;
    }

    strcpy(shared, "Hello, Shared Memory!");

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        shmdt(shared);
        shmctl(shm_id, IPC_RMID, NULL);
        return 1;
    }

    if (pid == 0) {
        printf("Child received: %s\n", shared);
        shmdt(shared);
        return 0;
    }

    wait(NULL);

    shmdt(shared);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}