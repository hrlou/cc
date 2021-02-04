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
    check_space:
    for (; isspace(c) && c != EOF; c = fgetc(file_ptr));

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
            fseek(file_ptr, -1L, SEEK_CUR);
            c = '/';
        }
    }
    if (!isspace(c)) {
        return;
    }
    goto check_space;
}

token get_tok(void) {
    token t;
    t.i = 0;

    // fseek(file_ptr, -1L, SEEK_CUR);
    
    /* parenthesis aren't working right now */

    switch (c) {
        case '=':
            c = fgetc(file_ptr);
            if (c == '=') {
                t.s = "==";
                t.i = Eq;
            } else {
                t.s = "=";
                t.i = Assign;
                fseek(file_ptr, -1L, SEEK_CUR);
                c = '=';
            }
            break;
        case '!':
            c = fgetc(file_ptr);
            if (c == '=') {
                t.s = "!=";
                t.i = Ne;
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
                fseek(file_ptr, -1L, SEEK_CUR);
                c = '<';
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
                fseek(file_ptr, -1L, SEEK_CUR);
                c = '>';
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
                fseek(file_ptr, -1L, SEEK_CUR);
                c = '+';
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
                fseek(file_ptr, -1L, SEEK_CUR);
                c = '-';
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
        case ',':
            t.s = ",";
            t.i = Comma;
            break;
        case '(':
            t.s = "(";
            t.i = Lpar;
        case ')':
            t.s = ")";
            t.i = Rpar;
        case '{':
            t.s = "{";
            t.i = Lbr;
        case '}':
            t.s = "}";
            t.i = Rbr;
        case '[':
        case ']':
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
    if (c >= '0' && c <= '9') {
        for (; (c >= '0' && c <= '9'); j++) {
            // putchar(c);
            c = fgetc(file_ptr);
        }
        fseek(file_ptr, -j, SEEK_CUR);
        c = fgetc(file_ptr);
        t.s = (char*)malloc(j + 1);
        for (int i = 1; i < j; i++) {
            *(t.s+(i-1)) = c;
            c = fgetc(file_ptr);
        }
        t.i = Num;
    }

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') || (c == '$')) {
        for (; (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') || (c == '$'); j++) {
            // putchar(c);
            c = fgetc(file_ptr);
        }
        fseek(file_ptr, -j, SEEK_CUR);
        c = fgetc(file_ptr);
        t.s = (char*)malloc(j + 1);
        for (int i = 1; i < j; i++) {
            *(t.s+(i-1)) = c;
            c = fgetc(file_ptr);
        }
        
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
    // putchar(c);
    return t;
}

token* lexer(const char* file) {
    // static size_t bufsize = 32, p = 0;
    token t;
    if (!(file_ptr = fopen(file, "rt"))) {
        perror(file);
        return NULL;
    }
    while (c != EOF) {
        skip();
        t = get_tok();
        if (t.i >= 128) {
            printf("%s \'%s\'\n", CC_TYPES[t.i - 128], t.s);
        }
    }
    // file_store[file_size] = '\0';
    fclose(file_ptr);
    
}