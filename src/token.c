#include "token.h"
#include <stdlib.h>
#include <stdio.h>

token_T* init_tok(char* val, int type) {
    token_T* token = calloc(1, sizeof(struct TOKEN));
    token->val = val;
    token->type = type;
    return token;
}

const char* type_to_str(int type) {
    switch (type) {
        case TOK_NUM: return "decimal_constant"; 
        case TOK_HEX: return "hex_constant";
        case TOK_IDENTIFIER: return "identifier";
        case TOK_CHAR_LITERAL: return "char_literal";
        case TOK_STRING_LITERAL: return "string_literal";
        case TOK_ASSIGN: return "assign";
        case TOK_SEMICOLON: return "semi";
        case TOK_COMMA: return "comma";
        case TOK_COLON: return "colon";
        case TOK_LEFT_PARENTHESIS: return "l_paren";
        case TOK_RIGHT_PARENTHESIS: return "r_paren";
        case TOK_LEFT_BRACKET: return "l_bracket";
        case TOK_RIGHT_BRACKET: return "r_bracket";
        case TOK_LEFT_BRACE: return "l_brace";
        case TOK_RIGHT_BRACE: return "r_brace";
        case TOK_LOGICAL_NOT: return "logical_not";
        case TOK_LOGICAL_AND: return "logical_and";
        case TOK_LOGICAL_OR: return "logical_or";
        case TOK_COMP_EQUAL: return "equal_to";
        case TOK_COMP_NOT_EQUAL: return "not_equal_to";
        case TOK_COMP_GREATER: return "greater_than";
        case TOK_COMP_LESS: return "less_than";
        case TOK_COMP_GREATER_EQUAL: return "greater_than_equal";
        case TOK_COMP_LESS_EQUAL: return "less_than_equal";
        case TOK_INCREMENT: return "increment";
        case TOK_DEINCREMENT: return "de-increment";
        case TOK_ADD: return "add";
        case TOK_SUBTRACT: return "subtract";
        case TOK_MULTIPLY: return "multiply";
        case TOK_DIVIDE: return "divide";
        case TOK_MODULO: return "modulo";
        case TOK_ADD_EQUAL: return "add_eq";
        case TOK_SUBTRACT_EQUAL: return "sub_eq";
        case TOK_MULTIPLY_EQUAL: return "multiple_eq";
        case TOK_DIVIDE_EQUAL: return "divide_eq";
        case TOK_MODULO_EQUAL: return "modulo_eq";
        case TOK_BIT_NOT: return "bitwise_not";
        case TOK_BIT_AND: return "bitwise_and";
        case TOK_BIT_OR: return "bitwise_or";
        case TOK_BIT_XOR: return "bitwise_xor";
        case TOK_BIT_SHIFT_LEFT: return "bitwise_l_shift";
        case TOK_BIT_SHIFT_RIGHT: return "bitwise_r_shift";
        case TOK_BIT_AND_EQUAL: return "bitwise_and_eq";
        case TOK_BIT_OR_EQUAL: return "bitwise_or_eq";
        case TOK_BIT_XOR_EQUAL: return "bitwise_xor_eq";
        case TOK_BIT_SHIFT_LEFT_EQUAL: return "bitwise_l_shift_eq";
        case TOK_BIT_SHIFT_RIGHT_EQUAL: return "bitwise_r_shift_eq";
        case TOK_EOF: return "eof";
    }
    return "UNEXPECTED_TYPE";
}

const char* tok_to_str(token_T* tok) {
    return type_to_str(tok->type); 
}