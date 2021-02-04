#ifndef LEXER_H
#define LEXER_H

/* reserved keywords
    auto
    break
    case
    char
    const
    continue
    default
    do
    double
    else
    enum
    extern
    float
    for
    goto
    if
    int
    long
    register
    return
    short
    signed
    sizeof
    static
    struct
    switch
    typedef
    union
    unsigned
    void
    volatile
    while
*/


/*  ! " # % & ' ( ) * + , - . / : ; < = > ? [ \ ] ^ _ { | } ~ 
    I really need to rewrite all of these to include the full C89 standard
*/

enum {
    Num = 128, Hex, Fun, Sys, Glo, Id, ChL, StrL,
    Semi, Comma, Col, Lpar, Rpar, Lbc, Rbc, Lbr, Rbr,
    Char, Int,
    Else, Enum, If, Return, Sizeof, While,
    Assign, Cond, Lor, Lan, Or, Xor, And, 
    Eq, Ne, Lt, Gt, Le, Ge, 
    Shl, Shr, Add, Sub, Mul, Div, Mod, 
    Inc, Dec,
    Eof
};

#ifdef __LEXER_C
const char* CC_TYPES[] = {
    "numeric_constant", "hex", "function", "Sys", "Glo", "identifier", "char_literal", "string_literal",
    "semicolon", "comma", "colon", "l_paren", "r_paren", "l_brace", "r_brace", "l_braket", "r_braket",
    "char", "int",
    "else", "enumerate", "if", "return", "sizeof", "while",
    "assign", "conditional", "Lor", "Lan", "Or", "Xor", "And", 
    "equal", "not_equal", "less_than", "greater_than", "less_equal", "greater_equal", 
    "shift_left", "shift_right", "add", "subtract", "multiply", "divide", "modulo", 
    "increment", "de-increment",
    "eof"
};
#endif // __LEXER_C

struct _lexer_token {
    char* s;
    unsigned int i;
};

typedef struct _lexer_token token;

token* lexer(const char* file);

#endif // LEXER_H