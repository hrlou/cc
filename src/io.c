#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file(const char* filename) {
    FILE* fp;
    size_t size;
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror(filename);
        exit(1);
    }
    fseek(fp, 0, SEEK_END); 
    size = ftell(fp); 
    rewind(fp);
    char* buf = (char*) calloc(size + 1, sizeof(char));
    fread(buf, sizeof(char), size, fp);
    buf[size] = '\0';
    fclose(fp);
    return buf;
}
