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
        /* skip compiler stuff for now */
        for (; lex->c != '\n'; lexer_advance(lex));
    } else if (lex->c == '/') {
        /* comments */
        if (lexer_peek(lex, 1) == '/') {
            /* C++ style */
            for (; lex->c != '\n' && lex->c != '\r'; lexer_advance(lex));
        } else if (lexer_peek(lex, 1) == '*') {
            /* C Style */
            for (; (lex->c != '*') && (lexer_peek(lex, 1) != '/'); lexer_advance(lex));
        }
    }
    if (!isspace(lex->c)) {
        return;
    }
    goto check_exit;

}

/* parse identifiers */
token_T* lexer_parse_id(lexer_T* lex) {
    int type = Id;
    char* value = calloc(1, sizeof(char));
    for (; isalpha(lex->c) || lex->c == '_'; lexer_advance(lex)) {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lex->c, 0});
    }
    return init_tok(value, type);
}

token_T* lexer_parse_number(lexer_T* lex) {
    int type = Num;
    char* value = calloc(1, sizeof(char));
    for (; isdigit(lex->c); lexer_advance(lex)) {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lex->c, 0});
    }
    return init_tok(value, type);
}

token_T* lexer_parse_literal(lexer_T* lex) {
    char delim = lex->c;
    int type = (delim == '\"') ? StrL : ChL;
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

    if (isalpha(lex->c)) {
        return lexer_parse_id(lex);
    }

    switch (lex->c) {
        case ';': return lexer_advance_tok(lex, init_tok(";", Semi));
        case ',': return lexer_advance_tok(lex, init_tok(",", Comma));
        case '(': return lexer_advance_tok(lex, init_tok("(", Lpar));
        case ')': return lexer_advance_tok(lex, init_tok(")", Rpar));
        case '{': return lexer_advance_tok(lex, init_tok("{", Lbr));
        case '}': return lexer_advance_tok(lex, init_tok("}", Rbr));
        case ':': return lexer_advance_tok(lex, init_tok(":", Col));
        case '[': return lexer_advance_tok(lex, init_tok("[", Lbc));
        case ']': return lexer_advance_tok(lex, init_tok("]", Rbc));
        case '~': return lexer_advance_tok(lex, init_tok("~", Not));
        case '=': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("==", Eq));
            } else {
                return lexer_advance_tok(lex, init_tok("=", Assign));
            }
        }
        case '!': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("!=", Neq));
            } else {
                return lexer_advance_tok(lex, init_tok("!", LNot));
            }
        }
        case '<': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("<=", Lte));
            } else if (lexer_peek(lex, 1) == '<') {
                return lexer_advance_tok(lex, (lexer_peek(lex, 2) == '=') ? init_tok("<<=", ShlE) : init_tok("<<", Shl));
            } else {
                return lexer_advance_tok(lex, init_tok("<", Lt));
            }
        }
        case '>': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok(">=", Gte));
            } else if (lexer_peek(lex, 1) == '>') {
                return lexer_advance_tok(lex, (lexer_peek(lex, 2) == '=') ? init_tok(">>=", ShrE) : init_tok(">>", Shr));
            } else {
                return lexer_advance_tok(lex, init_tok(">", Gt));
            }
        }
        case '+': {
            if (lexer_peek(lex, 1) == '+') {
                return lexer_advance_tok(lex, init_tok("++", Inc));
            } else if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("+=", AddE));
            } else {
                return lexer_advance_tok(lex, init_tok("+", Add));
            }
        }
        case '-': {
            if (lexer_peek(lex, 1) == '-') {
                return lexer_advance_tok(lex, init_tok("--", Dec));
            } else if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("-=", SubE));
            } else {
                return lexer_advance_tok(lex, init_tok("-", Sub));
            }
        }
        case '&': {
            if (lexer_peek(lex, 1) == '&') {
                return lexer_advance_tok(lex, init_tok("&&", LAnd));
            } else if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("&=", AndE));
            } else {
                return lexer_advance_tok(lex, init_tok("&", And));
            }
        }
        case '|': {
            if (lexer_peek(lex, 1) == '|') {
                return lexer_advance_tok(lex, init_tok("||", Lor));
            } else if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("|=", OrE));
            } else {
                return lexer_advance_tok(lex, init_tok("|", Or));
            }
        }
        case '^': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("^=", XorE));
            } else {
                return lexer_advance_tok(lex, init_tok("^", Xor));
            }
        }
        case '*': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("*=", MulE));
            } else {
                return lexer_advance_tok(lex, init_tok("*", Mul));
            }
        }
        case '/': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("/=", DivE));
            } else {
                return lexer_advance_tok(lex, init_tok("/", Div));
            }
        }
        case '%': {
            if (lexer_peek(lex, 1) == '=') {
                return lexer_advance_tok(lex, init_tok("%=", ModE));
            } else {
                return lexer_advance_tok(lex, init_tok("%", Mod));
            }
        }
        case '\'':
        case '\"':
            return lexer_parse_literal(lex);
        case 0:
            return init_tok(0, Eof);
        default: {
            fprintf(stderr, "[lexer] unexpected token '%c'\n", lex->c);
        } break;
    }
    return init_tok(0, Eof);
}