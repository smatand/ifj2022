/**
* @file expr.h
*
* @brief Implementation of parser module for IFJ22
*/
#ifndef EXPR_H
#define EXPR_H

#include<stdio.h>
#include"scanner.h"


#define TABLE_SIZE 15 //size of precedence table

typedef enum{
    RULE1, //E->E+E
    RULE2, //E->(E)
    RULE3, //E->i
    RULE_ERROR,
}eRules_t;
/** @brief state rules */
typedef enum{
    E_STATE_START,

    RULE1_EXPECTED1, //E->E+E, E->E<E, ...
    RULE1_EXPECTED2,
    RULE1_EXPECTED3,

    RULE2_EXPECTED1, //E->(E)
    RULE2_EXPECTED2,
    RULE2_EXPECTED3,

    RULE3_EXPECTED1, //E->i
    RULESTATES_ERROR,
}eStates_t;

/** @brief struct of tokens that are valid in precedence table*/
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
    P_LEFT_PAREN,
    P_RIGHT_PAREN,
    P_ID,
    P_DOLLAR,
    P_SEMICOLON, 
    P_ERROR,
}precTokenType_t;


/**
 * @brief precedence table, used to decide which operation should occur
 * precedenceTable[token in the stack][incoming token]
 */
const char precedenceTable[TABLE_SIZE][TABLE_SIZE];

/**
 * @brief converting token type from scanner to own precedence token type,(for the table)
 * 
 * @param token pointer to token
 * @return precTokenType_t return the relevant token type
 */
precTokenType_t tokenTypeToeType(token_t *token);

/**
 * @brief if reduce operation occurs, we use reducing rules to reduce the expression
 * 
 * @param stack pointer to stack
 */
void exprReduce(eStack_t *stack);

//used for debugging
char *tokenTypeToStr(token_t *token);

/**
 * @brief pushes INDENT and then pushed item
 * 
 * @param stack pointer to stack
 * @param item pointer to item
 */
void exprShift(eStack_t *stack, eItem_t *item);
eRules_t exprFindRule(eStack_t *stack);

#endif /* EXPR_H */ 
