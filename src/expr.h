/**
* @file expr.h
*
* @brief Implementation of parser module for IFJ22
*/

#ifndef EXPR_H
#define EXPR_H
#define TABLE_SIZE 16



char precedence_table[TABLE_SIZE][TABLE_SIZE];
typedef enum{
    P_MUL, 
    P_PLUS,
    P_MINUS,
    P_DIV,
    P_DOT,
    P_LESS,
    P_GREATER,
    P_GREATER_EQUAL,
    P_LESS_EQUAL,
    P_COMPARISON,
    P_NEG_COMPARISON,
    P_ASSIGN,
    P_LEFT_PAREN,
    P_RIGHT_PAREN,
    P_ID,
    P_DOLLAR,
}prec_token_type_t;


#endif /* EXPR_H */ 
