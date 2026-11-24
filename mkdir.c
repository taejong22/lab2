#include <sys/stat.h>

int main(void) {
    return mkdir("test_dir", 0755) == -1 ? 1 : 0;
}