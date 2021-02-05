#include "main.h"
#include "lexer.h"
#include "parse.h"
#include "io.h"
#include "handle.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(const int argc, char *argv[]) {
	(argc <= 1) ? exit(1) : compile_file(argv[1]);
    return 0;
}
