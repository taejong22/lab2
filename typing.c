#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX_LENGTH 256

int total_errors = 0;
int total_chars = 0;
int total_sentences = 0;
double total_time = 0.0;

// 오타 개수 계산 (동일한 위치의 문자 비교)
int countErrors(const char* original, const char* input) {
    int errors = 0;

    size_t original_len = strlen(original);
    size_t input_len = strlen(input);

    size_t min_len = original_len < input_len
                   ? original_len : input_len;

    for (size_t i = 0; i < min_len; i++) {
        if (original[i] != input[i])
            errors++;
    }

    return errors;
}

// 문장 하나에 대한 타자 연습
void practice(const char* sentence) {
    char input[MAX_LENGTH];
    struct timespec start, end;

    printf("\n[%d] %s\n", ++total_sentences, sentence);
    printf("입력: ");
    fflush(stdout);

    clock_gettime(CLOCK_MONOTONIC, &start);

    if (fgets(input, sizeof(input), stdin) == NULL)
        return;

    clock_gettime(CLOCK_MONOTONIC, &end);

    // fgets()에 포함된 Enter 문자 제거
    input[strcspn(input, "\n")] = '\0';

    double elapsed =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    int errors = countErrors(sentence, input);

    total_errors += errors;
    total_chars += (int)strlen(input);
    total_time += elapsed;
}

int main(void) {
    // sentences 파일에서 타이핑 문장을 읽어옴
    int fd = open("sentences.txt", O_RDONLY);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    char sentence[MAX_LENGTH];
    char ch;
    int index = 0;

    printf("===== Typing Practice =====\n");

    ssize_t bytes_read;

    while ((bytes_read = read(fd, &ch, 1)) == 1) {

        // Windows 형식의 줄바꿈 문자 무시
        if (ch == '\r')
            continue;

        if (ch == '\n') {
            sentence[index] = '\0';

            // 빈 줄은 건너뛰기
            if (index > 0)
                practice(sentence);

            index = 0;
        }
        else if (index < MAX_LENGTH - 1) {
            sentence[index++] = ch;
        }
    }

    if (bytes_read == -1)
        perror("read");

    // 마지막 줄에 null 문자가 없는 경우
    if (index > 0) {
        sentence[index] = '\0';
        practice(sentence);
    }

    close(fd);

    printf("\n===== Final Result =====\n");
    printf("총 문장 수: %d\n", total_sentences);
    printf("총 오타 수: %d\n", total_errors);
    printf("총 소요 시간: %.2f초\n", total_time);

    double cpm = total_time > 0
               ? total_chars * 60.0 / total_time
               : 0.0;

    printf("평균 분당 타자수: %.2f타\n", cpm);

    return 0;
}