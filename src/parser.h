/**
 * Project: Translator of language IFJ22
 * @file parser.h
 * @author Andrej Smatana - xsmata03
 * @author János László Vasík - xvasik05
 * @author Martin Maršalek - xmarsa15
 * 
 * @brief Header file of parser interface
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include "scanner.h"
#include "error.h"
#include "symtable.h"
#include "sym_table_stack.h"
#include "token.h"
#include "generator.h"
#include "parser.h"

/** @brief Parser structure */
typedef struct Parser
{
	htab_t *globalSymTable;
	sym_stack_t *localSymStack;
	token_t *currentToken;
	token_t *nextToken;
	DLList_t *codeGen;

	int onParam;
	int onParamType;
	int onArg;

	int ifCounter;
	int whileCounter;

	htab_pair_t *definedFunc;
} Parser_t;

/**
 * @brief Initialize the parser
 *
 * @return Pointer to initialized parser, or NULL in case of failure
 */
Parser_t *initParser();


/**
 * @brief Free all memory allocated by the parser
 * @param parser struct to be freed
 */
void destroyParser(Parser_t *parser);

/**
 * @brief Get the next token from the scanner
 * @param parser struct for storing the token
 *
 * @return SUCCESS, otherwise ERR_INTERNAL or ERR_LEX_ANALYSIS 
 */
int getNextToken(Parser_t *parser);

/**
 * @brief Check the type of a token
 * @param token token to check
 * @param type type of token to check against
 *
 * @return 0 if token types match, otherwise 1
 */
int checkTokenType(token_t *token, tokenType_t type);

/**
 * @brief Check the keyword of a token
 * @param token token to check
 * @param keyword type of token to check against
 *
 * @return 0 if keywords match, otherwise 1
 */
int checkTokenKeyword(token_t *token, keyword_t keyword);

/**
 * @brief Main program
 * @param parser structure to work with
*/
int rProgram(Parser_t * parser);

#endif /* PARSER_H */