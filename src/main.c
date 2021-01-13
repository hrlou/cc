#include "main.h"
#include "lexer.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* read_file(const char* file) {
    FILE *file_open;
    if (!(file_open = fopen(file, "rt"))) {
        perror(file);
        return NULL;
    }
    // determine size needed for the
    fseek(file_open, 0, SEEK_END);
    size_t file_size = ftell(file_open);
    fseek(file_open, 0, SEEK_SET);
    char* file_store = (char*)malloc(file_size);

    for (int i = 0; i <= file_size; i++) {
        file_store[i] = fgetc(file_open);
    }
    // file_store[file_size] = '\0';
    fclose(file_open);
    return file_store;
}

int main(const int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "No input files\n");
        return -1;
    }
    for (int i = 1; argc > i; i++) {
        char* s = read_file(argv[i]);
        if (s != NULL) {
            lexer(s);
        }
    }
    return 0;
}