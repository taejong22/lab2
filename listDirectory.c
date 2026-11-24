#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void listDirectory(const char* path) {
    DIR* dir = opendir(path);

    if (dir == NULL) {
        perror(path);
        return;
    }

    struct dirent* entry;
    struct stat st;
    char full_path[4096];

    printf("\n%s:\n", path);

    // 현재 디렉토리 내부 항목 출력
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        printf("%s  ", entry->d_name);
    }

    printf("\n");

    // 다시 처음부터 탐색
    rewinddir(dir);

    // 하위 디렉토리 재귀 탐색
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(full_path, sizeof(full_path),
                 "%s/%s", path, entry->d_name);

        if (stat(full_path, &st) == -1)
            continue;

        if (S_ISDIR(st.st_mode))
            listDirectory(full_path);
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : ".";

    listDirectory(path);

    return 0;
}