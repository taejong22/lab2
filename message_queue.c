#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct Message {
    long type;
    char text[100];
};

int main(void) {
    int msg_id = msgget(IPC_PRIVATE, IPC_CREAT | 0600);

    if (msg_id == -1) {
        perror("msgget");
        return 1;
    }

    struct Message msg = {1, "Hello, Message Queue!"};

    msgsnd(msg_id, &msg, strlen(msg.text) + 1, 0);

    struct Message received;

    msgrcv(msg_id, &received, sizeof(received.text), 1, 0);

    printf("Received: %s\n", received.text);

    msgctl(msg_id, IPC_RMID, NULL);

    return 0;
}