#include "token.h"
#include <stdlib.h>

token_T* init_tok(char* val, int type) {
    token_T* token = calloc(1, sizeof(struct TOKEN));
    token->val = val;
    token->type = type;
    return token;
}

const char* type_to_str(int type) {
    switch (type) {
        case Num: return "decimal_constant"; 
        case Hex: return "hex_constant";
        case Id: return "identifier";
        case ChL: return "char_literal";
        case StrL: return "string_literal";
        case Assign: return "assign";
        case Semi: return "semi";
        case Comma: return "comma";
        case Col: return "colon";
        case Lpar: return "l_paren";
        case Rpar: return "r_paren";
        case Lbc: return "l_bracket";
        case Rbc: return "r_bracket";
        case Lbr: return "l_brace";
        case Rbr: return "r_brace";
        case LNot: return "logical_not";
        case LAnd: return "logical_and";
        case Lor: return "logical_or";
        case Eq: return "equal_to";
        case Neq: return "not_equal_to";
        case Gt: return "greater_than";
        case Lt: return "less_than";
        case Gte: return "greater_than_equal";
        case Lte: return "less_than_equal";
        case Inc: return "increment";
        case Dec: return "de-increment";
        case Add: return "add";
        case Sub: return "subtract";
        case Mul: return "multiply";
        case Div: return "divide";
        case Mod: return "modulo";
        case AddE: return "add_eq";
        case SubE: return "sub_eq";
        case MulE: return "multiple_eq";
        case DivE: return "divide_eq";
        case ModE: return "modulo_eq";
        case Not: return "bitwise_not";
        case And: return "bitwise_and";
        case Or: return "bitwise_or";
        case Xor: return "bitwise_xor";
        case Shl: return "bitwise_l_shift";
        case Shr: return "bitwise_r_shift";
        case AndE: return "bitwise_and_eq";
        case OrE: return "bitwise_or_eq";
        case XorE: return "bitwise_xor_eq";
        case ShlE: return "bitwise_l_shift_eq";
        case ShrE: return "bitwise_r_shift_eq";
        case Eof: return "eof";
    }
    return "UNEXPECTED_TYPE";
}

const char* tok_to_str(token_T* tok) {
    return type_to_str(tok->type); 
}