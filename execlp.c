#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("Before execlp()\n");
    fflush(stdout);

    execlp("ls", "ls", "-l", NULL);

    perror("execlp");
    return 1;
}