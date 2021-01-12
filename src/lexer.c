#include "lexer.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

char* sanitise(char* i) {
    size_t bufsize = 128, p = 0;
    char *s = malloc(bufsize * sizeof(char*));
    /*  purpose of this is to convert any possible valid formatting into something readable by the tokeniser
        we need to use the realloc method because there is a very likely chance the size of the original i will exceed the output */
    for (; *i; i++) {
        while (*i == '#') {
            // skip compiler stuff for now
            for (; *i != '\n'; i++);
        }
        if (isspace(*i)) {
            // putchar(*(i-1));
            while (isspace(*(i+1))) {
                i++;
            }
            *i = ' ';
        } 
        switch (*i) {
            case '{':
            case '}':
            case '(':
            case ')':
            case ',':
            case '<':
            case '>':
            case '=':
            case '+':
            case '-':
            case '*':
            case '/':
            case ';':
            case '!':
            case '.':
                if (*i == '/') {
                    if (*(i+1) == '*') {
                        i+=2;
                        for (; *i != '*' && *(i+1) != '/'; i++);
                    } else if (*(i+1) == '/') {
                        for (; *i != '\n'; i++);
                    }
                } else {
                    if (s[p-1] == ' ') {
                        s[p-1] = *i;
                    } else {
                        s[p] = *i;
                        p++;
                    }
                }
                for (; isspace(*(i+1)); i++); 
                break;
            case '\'':
            case '"':
                s[p] = *i;
                p++;
                for (; *i != '\'' && *i != '"'; p++) {
                    s[p] = *i;
                }
                break;
            default:
                if (isspace(*i)) {
                    s[p] = ' ';
                } else {
                    s[p] = *i;
                }
                s[p] = *i;  
                p++;
                break;
        }
        // resize the pointer if it overflows
        if (p >= bufsize) {
            bufsize += bufsize;
            s = realloc(s, bufsize * sizeof(char*));
        }
    }
    s[p-1] = 0;
    return s;
}

char* lexer(char* i) {
    char* s = sanitise(i);
    free(i);
    puts(s);
    /*
    type -> ident -> ( is function 
    type -> ident -> = or ; is variable

    */

    /*for (; *s; s++) {

        putchar(*s);
    }*/

    return s;
}