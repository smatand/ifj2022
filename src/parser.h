/**
 * @file parser.h
 *
 * @brief Parser interface
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include "scanner.h"

#define CALL_RULE(ruleFunction)     \
	do                              \
	{                               \
		int error = ruleFunction(); \
		if (error != 0)             \
			return error;           \
	} while (0)

typedef struct Parser
{
	// TODO: symtable, bools, etc.
	scanner_t* scanner;
} Parser_t;

/**
 * @brief Initialize the parser
 * @param
 * @return Error code
 */
Parser_t* initParser();

/**
 * @brief Free all memory allocated by the parser
 * @param
 * @return Error code
 */
int destroyParser();

/**
 * @brief Get the next token from the scanner
 * @param
 * @return Error code
 */
int getNextToken();

/**
 * @brief Check the type of the current token
 * @param TokenType Type of token to check against
 * @return True if token types match, otherwise false
 */
bool checkTokenType(TokenType type);

/**
 * @brief Parse the source code
 * @param
 * @return Error code
 */
int parseSource();

//###############################~~~RULES~~~#######

// Rule #1
int rProgram();
// Rule #2
int rProlog();
// Rule #3
int rUnits();
// Rules #5, #6
int rUnit();
// Rule #7
int rFunctionDefinition();
// Rule #9
int rParams();
// Rule #11
int rParam();
// Rule #12
int rParam_n();
// Rules #14, #15, #16
int rType();
// Rules #17, #18, #19, #20, #21
int rStatements();
// Rule #23
int rAssignmentStatement();
// Rule #24
int rConditionalStatement();
// Rule #25
int rWhileLoopStatement();
// Rule #26
int rFunctionCallStatement();
// Rule #27
int rReturnStatement();
// Rules #28, #29
int rAssignment();
// Rule #30
int rArguments();
// Rule #32
int rArgument_n();
// Rule #34
int rReturnValue();
// Rules #36, #37, #38, #39, #40
int rTerm();
// Rule #41
int rProgramEnd();

//#################################################

#endif /* PARSER_H */