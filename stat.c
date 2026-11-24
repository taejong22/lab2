#include <stdio.h>
#include <sys/stat.h>

int main(void) {
    struct stat st;

    if (stat("test.txt", &st) == -1)
        return 1;

    printf("File size: %lld bytes\n", (long long)st.st_size);
    printf("File mode: %o\n", (unsigned int)(st.st_mode & 0777));

    return 0;
}