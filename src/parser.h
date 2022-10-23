/**
 * @file parser.h
 *
 * @brief Parser interface
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include "scanner.h"
#include "error.h"
#include "symtable.h"

#define INITIAL_BUCKET_COUNT 113

/** @brief Parser structure */
typedef struct Parser
{
	// TODO: bools, etc.
	htab_t *symTable;
	token_t *currentToken;
	token_t *nextToken;
} Parser_t;

/**
 * @brief Initialize the parser
 *
 * @return Pointer to initialized parser, or NULL in case of failure
 */
Parser_t *initParser();

/**
 * @brief Free all memory allocated by the parser
 * @param
 *
 * @return Error code
 */
int destroyParser(Parser_t *parser);

/**
 * @brief Get the next token from the scanner
 *
 * @return Error code
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
 * @param keyword Type of token to check against
 *
 * @return 0 if keywords match, otherwise 1
 */
int checkTokenKeyword(token_t *token, keyword_t keyword);

/**
 * @brief Parse the source code
 * @param
 *
 * @return Error code
 */
int parseSource(Parser_t *parser);

//###############################~~~RULES~~~#######

// Rule #1
int rProgram(Parser_t *parser);
// Rule #2
int rProlog(Parser_t *parser);
// Rule #3
int rUnits(Parser_t *parser);
// Rules #5, #6
int rUnit(Parser_t *parser);
// Rule #7
int rFunctionDefinition(Parser_t *parser);
// Rule #9
int rParams(Parser_t *parser);
// Rule #11
int rParam(Parser_t *parser);
// Rule #12
int rParam_n(Parser_t *parser);
// Rules #14, #15, #16
int rType(Parser_t *parser);
// Rules #17, #18, #19, #20, #21
int rStatements(Parser_t *parser);
// Rule #23
int rAssignmentStatement(Parser_t *parser);
// Rule #24
int rConditionalStatement(Parser_t *parser);
// Rule #25
int rWhileLoopStatement(Parser_t *parser);
// Rule #26
int rFunctionCallStatement(Parser_t *parser);
// Rule #27
int rReturnStatement(Parser_t *parser);
// Rules #28, #29
int rAssignment(Parser_t *parser);
// Rule #30
int rArguments(Parser_t *parser);
// Rule #32
int rArgument_n(Parser_t *parser);
// Rule #34
int rReturnValue(Parser_t *parser);
// Rules #36, #37, #38, #39, #40
int rTerm(Parser_t *parser);
// Rule #41
int rProgramEnd(Parser_t *parser);

//#################################################

#endif /* PARSER_H */