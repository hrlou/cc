#include <stdio.h>
#include "io.h"
#include "lexer.h"
#include "token.h"


void compile_src(char* s) {
	lexer_T* lex = lexer_init(s);
	for (; lex->c; lexer_advance(lex)) {
		lexer_skip(lex);
		putchar(lex->c);
	}
}

void compile_file(const char* filename) {
	compile_src(read_file(filename));
}