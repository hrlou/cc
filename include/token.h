#ifndef TOKEN_H
#define TOKEN_H

/*  ! " # % & ' ( ) * + , - . / : ; < = > ? [ \ ] ^ _ { | } ~   */
/*  = ; , : ( ) [ ] { } ! && || == ! => < >= <= ++ -- + - * / % ~ & | ^ << >> &= |= ^= <<= >>= */
typedef struct TOKEN {
    char* val;
    enum {
        /* syntax */
        Num, Hex, Id, ChL, StrL, Assign, 
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
    } type ;
} token_T;


const char* type_to_str(int type);
const char* tok_to_str(token_T* tok);

#endif /* TOKEN_H */
