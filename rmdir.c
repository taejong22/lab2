#include <unistd.h>

int main(void) {
    return rmdir("test_dir") == -1 ? 1 : 0;
}