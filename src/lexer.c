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

/* see the <offset> spaces ahead */
char lexer_peek(lexer_T* lex, int offset) {
    return lex->s[((lex->i + offset) < (lex->max)) ? (lex->i + offset) : (lex->max)];
}

token_T* lexer_advance_with(lexer_T* lex, token_T* tok);
token_T* lexer_advance_current(lexer_T* lex, int type);

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

token_T* lexer_parse_id(lexer_T* lex);
token_T* lexer_parse_number(lexer_T* lex);
token_T* lexer_parse_hex(lexer_T* lex);
token_T* lexer_parse_literal(lexer_T* lex);

token_T* lexer_next_token(lexer_T* lex) {

}

/*token get_tok(void) {
    token t;
    t.i = 0;
    int j = 1;
    if (isreserved(c)) {
        switch (c) {
            case ';':
                t.s = ";";
                t.i = Semi;
                break;
            case ',':
                t.s = ",";
                t.i = Comma;
                break;
            case '(':
                t.s = "(";
                t.i = Lpar;
                break;
            case ')':
                t.s = ")";
                t.i = Rpar;
                break;
            case '{':
                t.s = "{";
                t.i = Lbc;
                break;
            case '}':
                t.s = "}";
                t.i = Rbc;
                break;
            case ':':
                t.s = ":";
                t.i = Col;
                break;
            case '[':
                t.s = "[";
                t.i = Lbr;
                break;
            case ']':
                t.s = "]";
                t.i = Rbr;
                break;
            case '~':
                t.s = "~";
                t.i = Not;
                break;
            case '=':
                c = fgetc(file_ptr);
                if (c == '=') {
                    t.s = "==";
                    t.i = Eq;
                } else {
                    t.s = "=";
                    t.i = Assign;
                    ungetc(c, file_ptr);
                }
                break;
            case '!':
                c = fgetc(file_ptr);
                if (c == '=') {
                    t.s = "!=";
                    t.i = Neq;
                } else {
                    t.s = "!";
                    t.i = LNot;
                    ungetc(c, file_ptr);
                }
                break;
            case '<':
                c = fgetc(file_ptr);
                if (c == '=') {
                    t.s = "<=";
                    t.i = Lte;
                } else if (c == '<') {
                    c = fgetc(file_ptr);
                    if (c == '=') {
                        t.s = "<<=";
                        t.i = ShlE;
                        break;
                    }
                    ungetc(c, file_ptr);
                    t.s = "<<";
                    t.i = Shl;
                } else {
                    t.s = "<";
                    t.i = Lt;
                    ungetc(c, file_ptr);
                }
                break;
            case '>':
                c = fgetc(file_ptr);
                if (c == '=') {
                    t.s = ">=";
                    t.i = Gte;
                } else if (c == '>') {
                    c = fgetc(file_ptr);
                    if (c == '=') {
                        t.s = ">>=";
                        t.i = ShrE;
                        break;
                    }
                    ungetc(c, file_ptr);
                    t.s = ">>";
                    t.i = Shr;
                } else {
                    t.s = ">";
                    t.i = Gt;
                    ungetc(c, file_ptr);
                }
                break;
            case '+':
                c = fgetc(file_ptr);
                if (c == '+') {
                    t.s = "++";
                    t.i = Inc;
                } else if (c == '=') {
                    t.s = "+=";
                    t.i = AddE;
                } else {
                    t.s = "+";
                    t.i = Add;
                    ungetc(c, file_ptr);
                }
                break;
            case '-':
                c = fgetc(file_ptr);
                if (c == '-') {
                    t.s = "--";
                    t.i = Dec;
                } else if (c == '=') {
                    t.s = "+=";
                    t.i = SubE;
                } else {
                    t.s = "-";
                    t.i = Sub;
                    ungetc(c, file_ptr);
                }
                break;
            case '&':
                c = fgetc(file_ptr);
                if (c == '&') {
                    t.s = "&&";
                    t.i = LAnd;
                } else if (c == '=') {
                    t.s = "&=";
                    t.i = AndE;
                } else {
                    t.s = "&";
                    t.i = And;
                    ungetc(c, file_ptr);
                }
                break;
            case '|':
                c = fgetc(file_ptr);
                if (c == '|') {
                    t.s = "||";
                    t.i = Lor;
                } else if (c == '=') {
                    t.s = "|=";
                    t.i = OrE;
                } else {
                    t.s = "|";
                    t.i = Or;
                    ungetc(c, file_ptr);
                }
                break;
            case '^':
                c = fgetc(file_ptr);
                if (c == '=') {
                    t.s = "^=";
                    t.i = XorE;
                } else {
                    t.s = "^";
                    t.i = Xor;
                    ungetc(c, file_ptr);
                }
                break;
            case '*':
                c = fgetc(file_ptr);
                if (c == '=') {
                    t.s = "*=";
                    t.i = MulE;
                } else {
                    t.s = "*";
                    t.i = Mul;
                    ungetc(c, file_ptr);
                }
                break;
            case '/':
                c = fgetc(file_ptr);
                if (c == '=') {
                    t.s = "/=";
                    t.i = DivE;
                } else {
                    t.s = "/";
                    t.i = Div;
                    ungetc(c, file_ptr);
                }
                break;
            case '%':
                c = fgetc(file_ptr);
                if (c == '=') {
                    t.s = "%=";
                    t.i = ModE;
                } else {
                    t.s = "%";
                    t.i = Mod;
                    ungetc(c, file_ptr);
                }
                break;
        }
    } else if (c == EOF) {
        t.s = "";
        t.i = Eof;
    } else if (c == '"' || c == '\'') {
        t.i = c == '"' ? StrL : ChL;
        int delim = c;
        do {
            c = fgetc(file_ptr);
            j++;
        } while (c != delim);
        fseek(file_ptr, -j, SEEK_CUR);
        t.s = (char*)malloc(j + 1);
        fread(t.s, j, 1, file_ptr);
    } else if (c >= '0' && c <= '9') {
        for (; (c >= '0' && c <= '9'); j++) {
            c = fgetc(file_ptr);
        }
        fseek(file_ptr, -j, SEEK_CUR);
        t.s = (char*)malloc(j + 1);
        fread(t.s, j - 1, 1, file_ptr);
        t.i = Num;
    } else if (!isreserved(c)) {
        for (; (!isreserved(c) && !isspace(c)) || (c == '_'); j++) {
            c = fgetc(file_ptr);
        }
        fseek(file_ptr, -j, SEEK_CUR);
        t.s = (char*)malloc(j + 1);
        fread(t.s, j - 1, 1, file_ptr);
        t.i = Id;

        if (strcmp(t.s, "return") == 0) {
            t.i = Return;
        } else if (strcmp(t.s, "if") == 0) {
            t.i = If;
        } else if (strcmp(t.s, "else") == 0) {
            t.i = Else;
        } else if (strcmp(t.s, "while") == 0) {
            t.i = While;
        } else if (strcmp(t.s, "sizeof") == 0) {
            t.i = Sizeof;
        } else if (strcmp(t.s, "int") == 0) {
            t.i = Int;
        } else if (strcmp(t.s, "char") == 0) {
            t.i = Char;
        }
    }
    // fseek(file_ptr, -1L, SEEK_CUR);
    return t;
}*/
