/**
 * Project: IFJ22 language Compiler
 * @file expr.h
 * @author Andrej Smatana - xsmata03
 * @author Tomáš Frátrik - xfratr01
 *
 * @brief Header file for precedence analyzer unit
 */

#ifndef EXPR_H
#define EXPR_H

#include <stdio.h>
#include "scanner.h"
#include "expr_stack.h"
#include "parser.h"

#define TABLE_SIZE 15 // size of precedence table

/** @brief rules */
typedef enum
{
    RULE1, // E->E+E
    RULE2, // E->(E)
    RULE3, // E->i
    RULE_ERROR,
} eRules_t;
/** @brief state rules */
typedef enum
{
    E_STATE_START,

    RULE1_EXPECTED1, // E->E+E, E->E<E, ...
    RULE1_EXPECTED2,
    RULE1_EXPECTED3,

    RULE2_EXPECTED1, // E->(E)
    RULE2_EXPECTED2,
    RULE2_EXPECTED3,

    RULE3_EXPECTED1, // E->i
    RULESTATES_ERROR,
} eStates_t;

/** @brief type of tokens that are valid in precedence table*/
typedef enum
{
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
} precTokenType_t;

/**
 * @brief Converts token type from scanner to own precedence token type (for the table)
 * @param token pointer to token
 *
 * @return the relevant token type
 */
precTokenType_t tokenTypeToeType(token_t *token);

/**
 * @brief Applies rules (eRules_t) to reduce current expression
 * @param stack pointer to stack
 * @param nonTermCnt variable for generating indexed tempvars in ifjcode
 * @param generateCode bool varible if we shoud generate code
 * @return int
 */
// int exprReduce(struct eStack *stack, size_t *nonTermCnt,bool generateCode, Parser_t *parser);
int exprReduce(eStack_t *stack, size_t *nonTermCnt, bool generateCode, Parser_t *parser);

/**
 * @brief Retuns type of terminal (debug function)
 * @param token pointer to terminal
 *
 * @return type of terminal
 */
char *tokenTypeToStr(token_t *token);

/**
 * @brief Finds the relevant rule
 * @param stack pointer to stack
 *
 * @return returns which the rule that fits
 */
eRules_t exprFindRule(eStack_t *stack);

/**
 * @brief Finds the closest term in the stack
 * @param stack pointer tu stack
 *
 * @return pointer to found eItem (terminal)
 */
struct eItem *findClosestTerm(eStack_t *stack);

/**
 * @brief Analyzes expressions by checking the incoming token and the content of the stack
 * @param firstToken pointer to first token of expression
 * @param secondToken pointer to second token of expression
 * @param returnToken integer value of last read char that is not part of expression
 *
 * @return SUCCESS, otherwise some error (ERR_*)
 */

/**
 * @brief Analyzes expressions by comparing the incoming token and the content of the stack
 * 
 * @param returnToken returning ending token
 * @param parser for scanning tokens
 * @param generateCode if we shall generate code or not
 * @return int returning error statement
 */
int exprParse(int *returnToken, Parser_t *parser,bool generateCode);

/**
 * @brief generate ending code for expression, determined by ending token
 *
 * @param nonTermCnt id of nonterm
 * @param generateCode shall we even generate code?
 * @param type type of ending token
 */
void exprGenerateEndingCode(size_t nonTermCnt, bool generateCode, int type);
#endif /* EXPR_H */
