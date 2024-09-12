#include <stdio.h>
#include <string.h>

char* getFileName(char* file_path) {
    char* fn = strrchr(file_path, '/');
    if (fn == NULL) {
        return file_path;
    } else if (strlen(fn) == 1) {
        file_path[strlen(file_path) - 1] = '\0';
        return getFileName(file_path);
    } else {
        return ++fn;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filepath>\n", argv[0]);
        return 1;
    }

    char* file_path = argv[1];
    char* file_name = getFileName(file_path);
    
    printf("File name: %s\n", file_name);

    return 0;
}
