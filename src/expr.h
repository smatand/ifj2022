/**
 * Project: Translator of language IFJ22
 * @file expr.h
 * @author Andrej Smatana - xsmata03
 * @author Tomáš Frátrik - xfratr01
 * 
 * @brief Header file for expression analyzer unit
 */

#ifndef EXPR_H
#define EXPR_H

#include <stdio.h>
#include "scanner.h"
#include "parser.h"

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


// /**
//  * @brief precedence table, used to decide which operation should occur
//  * precedenceTable[token in the stack][incoming token]
//  */
// const char precedenceTable[TABLE_SIZE][TABLE_SIZE];

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
 * @param nonTermCnt variable for generating indexed nonterms
 * @param generateCode bool varible if we shoud generate code
 * @return int 
 */
int exprReduce(struct eStack *stack, size_t *nonTermCnt,bool generateCode, Parser_t *parser);

//used for debugging
char *tokenTypeToStr(token_t *token);

/**
 * @brief finds one of relevant rules
 * 
 * @param stack pointer to stack    
 * @return eRules_t returns which rule has occured
 */
eRules_t exprFindRule(struct eStack *stack);
/**
 * @brief function that finds the closest term in the stack
 * 
 * @param stack pointer tu stack
 * @return struct eItem* pointer to found token
 */
struct eItem *findClosestTerm(struct eStack *stack);

/**
 * @brief function reads incoming tokens, till we find ';' or error occurs with 
 * term type ')' (this is for if, while statements)
 * 
 * @param firstToken pointer to first token of expression
 * @return token_t* returning token which ended expression
 */
int exprParse(token_t *firstToken, token_t *secondToken, int *returnToken, Parser_t *parser);
#endif /* EXPR_H */ 
