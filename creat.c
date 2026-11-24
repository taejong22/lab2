#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int fd = creat("test.txt", 0644);

    if (fd == -1)
        return 1;

    close(fd);
    return 0;
}