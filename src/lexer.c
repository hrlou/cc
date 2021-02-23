#include "lexer.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* initialise a lexer from the source code */
lexer_T* lexer_init(char* s) {
    lexer_T* lex = calloc(1, sizeof(struct LEXER));
    lex->s = s;
    lex->max = strlen(s);
    lex->i = 0;
    lex->c = s[lex->i];
    return lex;
}

/* advance the lexer one charecter */
void lexer_advance(lexer_T* lex) {
    if (lex->i < lex->max && lex->c != '\0') {
        lex->i += 1;
        lex->c = lex->s[lex->i];
    }
}

/* advance past the current token */
token_T* lexer_advance_tok(lexer_T* lex, token_T* tok) {
    lexer_jump(lex, strlen(tok->val));
    return tok;
}

/* jump the <offset> charecters ahead */
void lexer_jump(lexer_T* lex, int offset) {
    lex->i = ((lex->i + offset) > lex->max) ? lex->max : (lex->i + offset);
    lex->c = lex->s[lex->i];
}

/* see the <offset> charecters ahead */
char lexer_peek(lexer_T* lex, int offset) {
    return lex->s[((lex->i + offset) < (lex->max)) ? (lex->i + offset) : (lex->max)];
}

/* skip whitespace and comments */
void lexer_skip(lexer_T* lex) {
    /* I understand the stigma around gotos, I find it alot cleaner in this context */
    check_exit:
    for (; isspace(lex->c) && lex->c != '\0'; lexer_advance(lex));
    if (lex->c == EOF) {
        return;
    }

    if (lex->c == '#') {
        /* skip preproccesor stuff for now */
        for (; lex->c != '\n'; lexer_advance(lex));
    } else if (lex->c == '/') {
        /* comments */
        if (lexer_peek(lex, 1) == '/') {
            /* C++ style */
            for (; lex->c != '\n' && lex->c != '\r'; lexer_advance(lex));
        } else if (lexer_peek(lex, 1) == '*') {
            /* C Style */
            lexer_jump(lex, 2);
            for (; !((lex->c == '*') && (lexer_peek(lex, 1) == '/')); lexer_advance(lex));
            lexer_jump(lex, 2);

        }
    }
    if (!isspace(lex->c)) {
        return;
    }
    goto check_exit;

}

/* parse identifiers */
token_T* lexer_parse_id(lexer_T* lex) {
    int type = TOK_IDENTIFIER;
    char* value = calloc(1, sizeof(char));
    for (; isalpha(lex->c) || lex->c == '_'; lexer_advance(lex)) {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lex->c, 0});
    }
    return init_tok(value, type);
}

token_T* lexer_parse_number(lexer_T* lex) {
    int type = TOK_NUM;
    char* value = calloc(1, sizeof(char));
    for (; isdigit(lex->c) || isalpha(lex->c); lexer_advance(lex)) {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lex->c, 0});
    }
    /* dumb way of parsing hex */
    if (value[1] == 'x') {
        type = TOK_HEX;
    }
    return init_tok(value, type);
}

token_T* lexer_parse_literal(lexer_T* lex) {
    char delim = lex->c;
    int type = (delim == '\"') ? TOK_STRING_LITERAL : TOK_CHAR_LITERAL;
    char* value = calloc(1, sizeof(char));
    do {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lex->c, 0});
        lexer_advance(lex);
    } while (lex->c != delim);
    strcat(value, (char[]){delim, 0});
    lexer_advance(lex);
    return init_tok(value, type);
}

token_T* lexer_next_token(lexer_T* lex) {
    lexer_skip(lex);

    if (lex->c >= '0' && lex->c <= '9') {
        return lexer_parse_number(lex);
    }

    if (isalpha(lex->c) || lex->c == '_') {
        return lexer_parse_id(lex);
    }

    switch (lex->c) {
        case ';': return lexer_advance_tok(lex, init_tok(";", TOK_SEMICOLON));
        case ',': return lexer_advance_tok(lex, init_tok(",", TOK_COMMA));
        case '(': return lexer_advance_tok(lex, init_tok("(", TOK_LEFT_PARENTHESIS));
        case ')': return lexer_advance_tok(lex, init_tok(")", TOK_RIGHT_PARENTHESIS));
        case '{': return lexer_advance_tok(lex, init_tok("{", TOK_LEFT_BRACE));
        case '}': return lexer_advance_tok(lex, init_tok("}", TOK_RIGHT_BRACE));
        case ':': return lexer_advance_tok(lex, init_tok(":", TOK_COLON));
        case '[': return lexer_advance_tok(lex, init_tok("[", TOK_LEFT_BRACKET));
        case ']': return lexer_advance_tok(lex, init_tok("]", TOK_RIGHT_BRACKET));
        case '~': return lexer_advance_tok(lex, init_tok("~", TOK_BIT_NOT));
        case '=': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("==", TOK_COMP_EQUAL));
            } else {
                return lexer_advance_tok(lex, init_tok("=", TOK_ASSIGN));
            }
        }
        case '!': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("!=", TOK_COMP_NOT_EQUAL));
            } else {
                return lexer_advance_tok(lex, init_tok("!", TOK_LOGICAL_NOT));
            }
        }
        case '<': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("<=", TOK_COMP_LESS_EQUAL));
            } else if (lexer_peek(lex, 1) == '<') {
                return lexer_advance_tok(lex, (lexer_peek(lex, 2) == '=') ? init_tok("<<=", TOK_BIT_SHIFT_LEFT_EQUAL) : init_tok("<<", TOK_BIT_SHIFT_LEFT));
            } else {
                return lexer_advance_tok(lex, init_tok("<", TOK_COMP_LESS));
            }
        }
        case '>': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok(">=", TOK_COMP_GREATER_EQUAL));
            } else if (lexer_peek(lex, 1) == '>') {
                return lexer_advance_tok(lex, (lexer_peek(lex, 2) == '=') ? init_tok(">>=", TOK_BIT_SHIFT_RIGHT_EQUAL) : init_tok(">>", TOK_BIT_SHIFT_RIGHT));
            } else {
                return lexer_advance_tok(lex, init_tok(">", TOK_COMP_GREATER));
            }
        }
        case '+': {
            if (lexer_peek(lex, 1) == '+') {
                return lexer_advance_tok(lex, init_tok("++", TOK_INCREMENT));
            } else if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("+=", TOK_ADD_EQUAL));
            } else {
                return lexer_advance_tok(lex, init_tok("+", TOK_ADD));
            }
        }
        case '-': {
            if (lexer_peek(lex, 1) == '-') {
                return lexer_advance_tok(lex, init_tok("--", TOK_DEINCREMENT));
            } else if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("-=", TOK_SUBTRACT_EQUAL));
            } else {
                return lexer_advance_tok(lex, init_tok("-", TOK_SUBTRACT));
            }
        }
        case '&': {
            if (lexer_peek(lex, 1) == '&') {
                return lexer_advance_tok(lex, init_tok("&&", TOK_LOGICAL_AND));
            } else if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("&=", TOK_BIT_AND_EQUAL));
            } else {
                return lexer_advance_tok(lex, init_tok("&", TOK_BIT_AND));
            }
        }
        case '|': {
            if (lexer_peek(lex, 1) == '|') {
                return lexer_advance_tok(lex, init_tok("||", TOK_LOGICAL_OR));
            } else if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("|=", TOK_BIT_OR_EQUAL));
            } else {
                return lexer_advance_tok(lex, init_tok("|", TOK_BIT_OR));
            }
        }
        case '^': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("^=", TOK_BIT_XOR_EQUAL));
            } else {
                return lexer_advance_tok(lex, init_tok("^", TOK_BIT_XOR));
            }
        }
        case '*': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("*=", TOK_MULTIPLY_EQUAL));
            } else {
                return lexer_advance_tok(lex, init_tok("*", TOK_MULTIPLY));
            }
        }
        case '/': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("/=", TOK_DIVIDE_EQUAL));
            } else {
                return lexer_advance_tok(lex, init_tok("/", TOK_DIVIDE));
            }
        }
        case '%': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("%=", TOK_MODULO_EQUAL));
            } else {
                return lexer_advance_tok(lex, init_tok("%", TOK_MODULO));
            }
        }
        case '\'':
        case '\"':
            return lexer_parse_literal(lex);
        case 0:
            return init_tok(0, TOK_EOF);
        default: {
            fprintf(stderr, "[lexer] unexpected token '%c'\n", lex->c);
        } break;
    }
    return init_tok(0, TOK_EOF);
}