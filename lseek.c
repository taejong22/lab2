#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int fd = open("test.txt", O_RDONLY);
    if (fd == -1)
        return 1;

    char buf[100];

    lseek(fd, 7, SEEK_SET);

    ssize_t n = read(fd, buf, sizeof(buf));

    if (n > 0)
        write(STDOUT_FILENO, buf, n);

    close(fd);
    return 0;
}