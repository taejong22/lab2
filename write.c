#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    int fd = open("test.txt", O_WRONLY | O_TRUNC);
    if (fd == -1)
        return 1;

    const char *msg = "Hello, Linux!\n";

    write(fd, msg, strlen(msg));
    close(fd);

    return 0;
}