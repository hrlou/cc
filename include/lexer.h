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
    I will use seperate enums for syntax, reserved, and operators making them easily disernable
*/

enum {
    /* syntax */
    Num = 128, Hex, Id, ChL, StrL, Assign, 
    Semi, Comma, Col, Lpar, Rpar, Lbc, Rbc, Lbr, Rbr,
    /* logical */
    LNot, LAnd, Lor,
    /* comparison */
    Eq, Neq, Gt, Lt, Gte, Lte,
    /*  arithmatic */
    Inc, Dec,
    Add, Sub, Mul, Div, Mod,
    /*  compound arithmatic */
    AddE, SubE, MulE, DivE, ModE,
    /*  bitwise */
    Not, And, Or, Xor, Shl, Shr,
    /*  compound bitwise */
    AndE, OrE, XorE, ShlE, ShrE,
    /* keywords */
    Eof
};

#ifdef __LEXER_C
/* = ; , : ( ) [ ] { } ! && || == ! => < >= <= ++ -- + - * / % ~ & | ^ << >> &= |= ^= <<= >>= */

const char* CC_TYPES[] = {
    /* syntax */
    "decimal_constant", "hex_constant", "identifier", "char_literal", "string_literal", "assign",
    "semi", "comma", "colon", "l_paren", "r_paren", "l_bracket", "r_bracket", "l_brace", "r_brace",
    /* logical */
    "logical_not", "logical_and", "logical_or",
    /* comparison */
    "equal_to", "not_equal_to", "greater_than", "less_than", "greater_than_equal", "less_than_equal",
    /*  arithmatic */
    "increment", "de-increment",
    "add", "subtract", "multiply", "divide", "modulo",
    /*  compound arithmatic */
    "add_eq", "sub_eq", "multiple_eq", "divide_eq", "modulo_eq",
    /*  bitwise */
    "bitwise_not", "bitwise_and", "bitwise_or", "bitwise_xor", "bitwise_l_shift", "bitwise_r_shift",
    /*  compound bitwise */
    "bitwise_and_eq", "bitwise_or_eq", "bitwise_xor_eq", "bitwise_l_shift_eq", "bitwise_r_shift_eq",
    "eof"
};
#endif /* __LEXER_C */

struct token {
    char* s;
    unsigned int i;
};

typedef struct token token;

token* lexer(const char* file);

#endif /* LEXER_H */