#ifndef LEXER_H
#define LEXER_H

#ifdef __LEXER_C
enum {
    Num = 128, Fun, Sys, Glo, Loc, Id, Literal,
    Char, Int,
    Else, Enum, If, Return, Sizeof, While,
    Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};

const char* CC_TYPES[] = {
    "number", "function", "Sys", "Glo", "Loc", "identifier", "literal",
    "char", "int",
    "Else", "Enum", "if", "return", "sizeof", "while",
    "assign", "conditional", "Lor", "Lan", "Or", "Xor", "And", "equal", "not_equal", "less_than", "greater_than", "less_equal", "greater_equal", "shift_left", "shift_right", "add", "subtract", "multiply", "divide", "modulo", "increment", "de-increment", "Brak"
};
#endif // __LEXER_C

struct lex {
    char** tok;
    int* id;
};

struct lex lexer(char* i);

#endif // LEXER_H