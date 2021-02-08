#include <stdio.h>
#include "io.h"
#include "lexer.h"
#include "token.h"

void compile_src(char* s) {
	lexer_T* lex = lexer_init(s);
	token_T* tok;
	do {
		tok = lexer_next_token(lex);
		puttok(tok);
	} while (tok->type != Eof);
}

void compile_file(const char* filename) {
	compile_src(read_file(filename));
}