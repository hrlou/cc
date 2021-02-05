#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include "token.h"

typedef struct LEXER {
  char* s;
  size_t max;
  char c;
  unsigned int i;
} lexer_T;

lexer_T* lexer_init(char* s);
void lexer_advance(lexer_T* lex);
char lexer_peek(lexer_T* lex, int offset);
token_T* lexer_advance_with(lexer_T* lex, token_T* tok);
token_T* lexer_advance_current(lexer_T* lex, int type);
void lexer_skip(lexer_T* lex);
token_T* lexer_parse_id(lexer_T* lex);
token_T* lexer_parse_number(lexer_T* lex);
token_T* lexer_parse_hex(lexer_T* lex);
token_T* lexer_parse_literal(lexer_T* lex);
token_T* lexer_next_token(lexer_T* lex);

#endif /* LEXER_H */
