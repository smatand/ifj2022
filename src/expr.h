/**
* @file expr.h
*
* @brief Implementation of parser module for IFJ22
*/

#ifndef EXPR_H
#define EXPR_H

#include<stdio.h>
#include"scanner.h"


#define TABLE_SIZE 16


typedef enum{
    E_STATE_START,

    RULE1_EXPECTED1,
    RULE1_EXPECTED2,
    RULE1_EXPECTED3,

    RULE2_EXPECTED1,
    RULE2_EXPECTED2,
    RULE2_EXPECTED3,

    RULE3_EXPECTED1,
}eStates_t;


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
    P_SEMICOLON,
    P_ERROR,
}precTokenType_t;

// ; )
//, ->FUNEXP rozsirenie'

const char precedenceTable[TABLE_SIZE][TABLE_SIZE];

char *tokenTypeToStr(TokenType type);
precTokenType_t tokenTypeToeType(TokenType type);
void exprReduce(eStack_t *stack);

#endif /* EXPR_H */ 
