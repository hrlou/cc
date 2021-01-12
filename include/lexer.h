#ifndef LEXER_H
#define LEXER_H

struct func_details {
    char* ident;
    char** tokens;
} ;

typedef struct func_details fcd;

struct file_details {
    int fcount;
    char** compiler;
    fcd functions;
} ;

typedef struct file_details fld;

char* lexer(char* input);

#endif // LEXER_H