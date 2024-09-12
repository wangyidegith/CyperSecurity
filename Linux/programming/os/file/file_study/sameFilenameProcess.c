#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

static int openFileToWrite(const char* filename, const int filename_len);
static bool judgeFileExisted(const char* filepath);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("请在命令行参数位置输入文件路径\n");
        return 1;
    }

    const char* filepath = argv[1];
    int fd = openFileToWrite(filepath, strlen(filepath));

    if (fd == -1) {
        printf("无法打开文件\n");
        return 1;
    }

    printf("成功打开文件\n");
    close(fd);

    return 0;
}

static int openFileToWrite(const char* filename, const int filename_len) {
    int fd;
    static int count = 0;
    
    if (judgeFileExisted(filename)) {
        size_t actual_len = (strlen(filename) + 20 + 1) * sizeof(char);
        char tmp[actual_len];
        memset(tmp, 0, actual_len);
        snprintf(tmp, actual_len, "%.*s%d", filename_len, filename, ++count);
        fd = openFileToWrite(tmp, filename_len);
    } else {
        fd = open(filename, O_CREAT | O_WRONLY, 0700);
    }

    if (fd == -1) {
        return -1;
    }

    return fd;
}

static bool judgeFileExisted(const char* filepath) {
    if (access(filepath, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}
