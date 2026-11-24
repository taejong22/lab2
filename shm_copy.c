#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

#define SHM_NAME "/shm_copy_test"

int main(void) {
    int src_fd = open("source.txt", O_RDONLY);

    if (src_fd == -1) {
        perror("open source");
        return 1;
    }

    // 원본 파일 크기 확인
    struct stat st;

    if (fstat(src_fd, &st) == -1) {
        perror("fstat");
        close(src_fd);
        return 1;
    }

    size_t size = st.st_size;

    if (size == 0) {
        printf("Source file is empty.\n");
        close(src_fd);
        return 1;
    }

    // POSIX 공유 메모리 생성
    int shm_fd = shm_open(
        SHM_NAME,
        O_CREAT | O_RDWR,
        0600
    );

    if (shm_fd == -1) {
        perror("shm_open");
        close(src_fd);
        return 1;
    }

    // 공유 메모리 크기 설정
    if (ftruncate(shm_fd, size) == -1) {
        perror("ftruncate");
        close(src_fd);
        close(shm_fd);
        shm_unlink(SHM_NAME);
        return 1;
    }

    // 공유 메모리 매핑
    void* memory = mmap(
        NULL,
        size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        shm_fd,
        0
    );

    if (memory == MAP_FAILED) {
        perror("mmap");
        close(src_fd);
        close(shm_fd);
        shm_unlink(SHM_NAME);
        return 1;
    }

    // 원본 파일을 공유 메모리에 읽기
    ssize_t bytes_read = read(src_fd, memory, size);

    if (bytes_read == -1) {
        perror("read");
        munmap(memory, size);
        close(src_fd);
        close(shm_fd);
        shm_unlink(SHM_NAME);
        return 1;
    }

    close(src_fd);

    // 복사 파일 생성
    int dst_fd = open(
        "copy.txt",
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );

    if (dst_fd == -1) {
        perror("open destination");
        munmap(memory, size);
        close(shm_fd);
        shm_unlink(SHM_NAME);
        return 1;
    }

    // 공유 메모리에서 복사 파일로 쓰기
    ssize_t bytes_written = write(dst_fd, memory, bytes_read);

    if (bytes_written == -1) {
        perror("write");
    }
    else {
        printf("File copied successfully!\n");
        printf("Copied bytes: %zd\n", bytes_written);
    }

    close(dst_fd);

    // 공유 메모리 정리
    munmap(memory, size);
    close(shm_fd);
    shm_unlink(SHM_NAME);

    return bytes_written == bytes_read ? 0 : 1;
}