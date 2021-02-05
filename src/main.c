#include "main.h"
#include "lexer.h"
#include "parse.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(const int argc, char *argv[]) {
    if (argc <= 1) {
        fputs("No input files\n", stderr);
        return 1;
    }
    parse(lexer(argv[1]));

    return 0;
}