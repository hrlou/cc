#ifndef TOKEN_H
#define TOKEN_H

#define puttok(TOK) \
    fprintf(stderr, "%s '%s'\n", type_to_str(TOK->type), TOK->val)

/*  ! " # % & ' ( ) * + , - . / : ; < = > ? [ \ ] ^ _ { | } ~   */
/*  = ; , : ( ) [ ] { } ! && || == ! => < >= <= ++ -- + - * / % ~ & | ^ << >> &= |= ^= <<= >>= */
typedef struct TOKEN {
    char* val;
    enum {
        /* syntax */
        TOK_NUM, TOK_HEX, TOK_IDENTIFIER, TOK_CHAR_LITERAL, TOK_STRING_LITERAL, TOK_ASSIGN, 
        TOK_SEMICOLON, TOK_COMMA, TOK_COLON, TOK_LEFT_PARENTHESIS, TOK_RIGHT_PARENTHESIS, TOK_LEFT_BRACKET, TOK_RIGHT_BRACKET, TOK_LEFT_BRACE, TOK_RIGHT_BRACE,
        /* logical */
        TOK_LOGICAL_NOT, TOK_LOGICAL_AND, TOK_LOGICAL_OR,
        /* comparison */
        TOK_COMP_EQUAL, TOK_COMP_NOT_EQUAL, TOK_COMP_GREATER, TOK_COMP_LESS, TOK_COMP_GREATER_EQUAL, TOK_COMP_LESS_EQUAL,
        /*  arithmatic */
        TOK_INCREMENT, TOK_DEINCREMENT,
        TOK_ADD, TOK_SUBTRACT, TOK_MULTIPLY, TOK_DIVIDE, TOK_MODULO,
        /*  compound arithmatic */
        TOK_ADD_EQUAL, TOK_SUBTRACT_EQUAL, TOK_MULTIPLY_EQUAL, TOK_DIVIDE_EQUAL, TOK_MODULO_EQUAL,
        /*  bitwise */
        TOK_BIT_NOT, TOK_BIT_AND, TOK_BIT_OR, TOK_BIT_XOR, TOK_BIT_SHIFT_LEFT, TOK_BIT_SHIFT_RIGHT,
        /*  compound bitwise */
        TOK_BIT_AND_EQUAL, TOK_BIT_OR_EQUAL, TOK_BIT_XOR_EQUAL, TOK_BIT_SHIFT_LEFT_EQUAL, TOK_BIT_SHIFT_RIGHT_EQUAL,
        /* keywords */
        TOK_EOF
    } type;
} token_T;

token_T* init_tok(char* val, int type);
const char* type_to_str(int type);
const char* tok_to_str(token_T* tok);

#endif /* TOKEN_H */
