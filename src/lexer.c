#define __LEXER_C
#include "lexer.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

FILE *file_ptr;
int c = 0;

void skip(void) {
    c = fgetc(file_ptr);
    /* I understand the stigma around gotos, I find it alot cleaner in this context */
    check_exit:
    for (; isspace(c) && c != EOF; c = fgetc(file_ptr));
    if (c == EOF) {
        return;
    }

    if (c == '#') {
        /* skip compiler stuff for now */
        for (; c != '\n'; c = fgetc(file_ptr));
    } else if (c == '/') {
        /* comments */
        c = fgetc(file_ptr);
        if (c == '/') {
            /* C++ style */
            for (; c != '\n'; c = fgetc(file_ptr));
        } else if (c == '*') {
            /* C Style */
            for (;;) {
                for (; c != '*'; c = fgetc(file_ptr));
                c = fgetc(file_ptr);
                if (c == '/') {
                    c = fgetc(file_ptr);
                    break;
                }
            }
        } else {
            ungetc(c, file_ptr);
        }
    }
    if (!isspace(c)) {
        return;
    }
    goto check_exit;
}

int isreserved(char ch) {
    const char r[] = "!\"#%&\'()*+,-./:;<=>?[\\]^_{|}~";
    return strchr(r, ch) ? 1 : 0;
}

token get_tok(void) {
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

        /* builtins */
        /*if (strcmp(t.s, "return") == 0) {
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
        }*/
    }

    /* need to stop from skipping stuff */
    // fseek(file_ptr, -1L, SEEK_CUR);
    return t;
}

token* lexer(const char* file) {
    static size_t bufsize = 16, pos = 0;
    token* t = (token*)malloc(bufsize * sizeof(token));
    if (!(file_ptr = fopen(file, "rt"))) {
        perror(file);
        return NULL;
    }
    while (c != EOF) {
        skip();
        t[pos] = get_tok();
        if (t[pos].i) {
            // printf("%s \'%s\'\n", CC_TYPES[t[pos].i - 128], t[pos].s);
            pos++;
        }
        if (pos >= bufsize) {
            bufsize *= 2;
            t = (token*)realloc(t, bufsize * sizeof(token));
        }
    }
    
    fclose(file_ptr);
    return t;
}