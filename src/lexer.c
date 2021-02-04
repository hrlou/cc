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

int see(const char* s) {
    char* buf = malloc(strlen(s) + 1);
    fread(buf, strlen(s), 1, file_ptr);
    int r = strcmp(buf, s) ? strlen(s) : 0;
    if (*buf) {
        puts(buf);
        free(buf);
    }
    return r;
}

token get_tok(void) {
    token t;
    t.i = 0;

    switch (c) {
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
                t.i = Ne;
            } else {
                ungetc(c, file_ptr);
            }
            break;
        case '<':
            c = fgetc(file_ptr);
            if (c == '=') {
                t.s = "<=";
                t.i = Le;
            } else if (c == '<') {
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
                t.i = Ge;
            } else if (c == '>') {
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
                
            } else {
                t.s = "-";
                t.i = Sub;
                ungetc(c, file_ptr);
            }
            break;
        case '*':
            t.s = "*";
            t.i = Mul;
            break;
        case '/':
            t.s = "/";
            t.i = Div;
            break;
        case '%':
            t.s = "%";
            t.i = Mod;
            break;
        case ';':
            t.s = ";";
            t.i = Semi;
            break;
        case ':':
            t.s = ":";
            t.i = Col;
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
        case '[':
            t.s = "[";
            t.i = Lbr;
            break;
        case ']':
            t.s = "]";
            t.i = Rbr;
            break;
        case EOF:
            t.s = "";
            t.i = Eof;
            break;
    }


    if (t.i != 0) {
        return t;
    }

    int j = 1;
    if (c == '"' || c == '\'') {
        t.i = c == '"' ? StrL : ChL;
        int delim = c;
        do {
            c = fgetc(file_ptr);
            j++;
        } while (c != delim);
        fseek(file_ptr, -j, SEEK_CUR);
        t.s = (char*)malloc(j + 1);
        fread(t.s, j, 1, file_ptr);
        c = fgetc(file_ptr);
    } else if (c >= '0' && c <= '9') {
        for (; (c >= '0' && c <= '9'); j++) {
            c = fgetc(file_ptr);
        }
        fseek(file_ptr, -j, SEEK_CUR);
        t.s = (char*)malloc(j + 1);
        fread(t.s, j - 1, 1, file_ptr);
        c = fgetc(file_ptr);
        t.i = Num;
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') || (c == '$')) {
        for (; (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') || (c == '$'); j++) {
            c = fgetc(file_ptr);
        }
        fseek(file_ptr, -j, SEEK_CUR);
        t.s = (char*)malloc(j + 1);
        fread(t.s, j - 1, 1, file_ptr);
        c = fgetc(file_ptr);
        t.i = Num;
        
        /* builtins */
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
        } else {
            t.i = Id;
        }
    }

    /* need to stop from skipping stuff */
    fseek(file_ptr, -1L, SEEK_CUR);
    return t;
}

token* lexer(const char* file) {
    static size_t bufsize = 16, pos = 0;
    token* t = malloc(bufsize * sizeof(token));
    if (!(file_ptr = fopen(file, "rt"))) {
        perror(file);
        return NULL;
    }
    while (c != EOF) {
        skip();
        t[pos] = get_tok();
        if (t[pos].i) {
            printf("%s \'%s\'\n", CC_TYPES[t[pos].i - 128], t[pos].s);
            pos++;
        }
        if (pos >= bufsize) {
            bufsize *= 2;
            t = realloc(t, bufsize * sizeof(token));
        }
    }
    
    fclose(file_ptr);

    for (int i = 0; t[i].i != Eof; i++) {
        if (t[i].i >= 128) {
            printf("%s \'%s\'\n", CC_TYPES[t[i].i - 128], t[i].s);
        }
    }
}