#define __LEXER_C
#include "lexer.h"


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct lex lexer(char* i) {
    size_t bufsize = 32, p = 0;
    // char *s = malloc(bufsize * sizeof(char*));
    struct lex t;
    t.tok = (char**)calloc(bufsize, sizeof(char*));
    t.id = (int*)calloc(bufsize, sizeof(int));
    /*  purpose of this is to convert any possible valid formatting into something readable by the tokeniser
        we need to use the realloc method because there is a very likely chance the size of the original i will exceed the output */


    for (int j; *i; i++) {
        while (*i == '#') {
            // skip compiler stuff for now
            for (; *i != '\n'; i++);
        }
        if (*i == '/') {
            if (*(i+1) == '*') {
                i+=2;
                for (; *i != '*' && *(i+1) != '/'; i++);
            } else if (*(i+1) == '/') {
                for (; *i != '\n'; i++);
            }
        }
        if (isspace(*i)) {
            // putchar(*(i-1));
            while (isspace(*(i+1))) {
                i++;
            }
            *i = ' ';
        }
        switch (*i) {
            // operators
            // https://www.tutorialspoint.com/cprogramming/c_operators.htm
            // ignoring bit operators for now
            //
            case '=':
                if (*(i+1) == '=') {
                    t.tok[p] = "==";
                    t.id[p] = Eq;
                    p++;
                    i++;
                } else {
                    t.tok[p] = "=";
                    t.id[p] = Assign;
                    p++;
                }
                break;
            case '!':
                if (*(i+1) == '=') {
                    t.tok[p] = "!=";
                    t.id[p] = Ne;
                    p++;
                    i++;
                }
                break;
            case '<':
                if (*(i+1) == '=') {
                    t.tok[p] = "<=";
                    t.id[p] = Le;
                    p++;
                    i++;
                } else {
                    t.tok[p] = "<";
                    t.id[p] = Lt;
                    p++;
                }
                break;
            case '>':
                if (*(i+1) == '=') {
                    t.tok[p] = ">=";
                    t.id[p] = Ge;
                    p++;
                    i++;
                } else {
                    t.tok[p] = ">";
                    t.id[p] = Gt;
                    p++;
                }
                break;
            // arithmatic
            case '+':
                if (*(i+1) == '+') {
                    t.tok[p] = "++";
                    t.id[p] = Inc;
                    p++;
                    i++;
                } else {
                    t.tok[p] = "+";
                    t.id[p] = Add;
                    p++;
                }
                break;
            case '-':
                if (*(i+1) == '-') {
                    t.tok[p] = "--";
                    t.id[p] = Dec;
                    p++;
                    i++;
                } else {
                    t.tok[p] = "-";
                    t.id[p] = Sub;
                    p++;
                }
                break;
            case '*':
                t.tok[p] = "*";
                t.id[p] = Mul;
                p++;
                break;
            case '/':
                t.tok[p] = "/";
                t.id[p] = Div;
                p++;
                break;
            case '%':
                t.tok[p] = "%";
                t.id[p] = Mod;
                p++;
                break;
            // other
            case '{':
            case '}':
            case '(':
            case ')':
            case ',':
            case ';':
                t.tok[p] = (char*)malloc(1);
                memset(t.tok[p], *i, 1);
                *(t.tok[p]+1) = 0;
                t.id[p] = 0;
                p++;
                break;
            case '\'':
            case '"':
                j = 1;
                for (; *(i+j) != '\'' && *(i+j) != '"'; j++);
                // printf("%.*s\n", j+1, i);
                t.tok[p] = (char*)malloc(j+2);
                memcpy(t.tok[p], i, j+1);
                *(t.tok[p]+j+1) = 0;
                t.id[p] = Literal;
                i+=j+1;
                p++;
                break;
        }
        if ((*i >= 'a' && *i <= 'z') || (*i >= 'A' && *i <= 'Z') || (*i == '_') || (*i == '$')) {
            j = 1;
            for (; (*(i+j) >= 'a' && *(i+j) <= 'z') || (*(i+j) >= 'A' && *(i+j) <= 'Z') || (*(i+j) == '_'); j++);
            
            t.tok[p] = (char*)malloc(j+1);
            memcpy(t.tok[p], i, j);
            *(t.tok[p]+j) = 0;
            i+=j;
            for (; *i == ' ' && *i == '(' && *i == ';'; i++);
            if (*i == '(') {
                t.id[p] = Fun;
            } else {
                t.id[p] = Id;
            }
            // builtins
            if (strcmp(t.tok[p], "return") == 0) {
                t.id[p] = Return;
            } else if (strcmp(t.tok[p], "if") == 0) {
                t.id[p] = If;
            } else if (strcmp(t.tok[p], "while") == 0) {
                t.id[p] = While;
            } else if (strcmp(t.tok[p], "sizeof") == 0) {
                t.id[p] = Sizeof;
            }

            // variables
            if (strcmp(t.tok[p], "int") == 0) {
                t.id[p] = Int;
            } else if (strcmp(t.tok[p], "char") == 0) {
                t.id[p] = Char;
            }
            p++;
        }
        if (p >= bufsize) {
            bufsize *= 2;
            t.tok = realloc(t.tok, bufsize * sizeof(char*));
            t.id = realloc(t.id, bufsize * sizeof(int));
        }
    }
    t.tok[p] = NULL;
    for (; *t.tok != NULL; t.tok++, t.id++) {
        // puts(*t.tok);
        if (*t.id >= 128) {
            printf("%s ", CC_TYPES[*t.id - 128]);
        }
        printf("\'%s\'\n", *t.tok);
    }
    return t;
}
/*  type -> ident -> ( is function 
    type -> ident -> = or ; is variable */