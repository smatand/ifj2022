/**
 * @file syntax_rules.h
 *
 * @brief Implementation of syntax rules for IFJ22
 */

#include "parser.h"
#include "parser_macros.h"

int rProgram(Parser_t *parser)
{
	CALL_RULE(rProlog);
	CALL_RULE(rUnits);
	CALL_RULE(rProgramEnd);
	return 0;
}

int rProlog(Parser_t *parser)
{
	CURRENT_TOKEN_TYPE(TOK_PROLOGUE);
	return 0;
}

int rUnits(Parser_t *parser)
{
	CALL_RULE(rUnit);
	return 0;
}

int rUnit(Parser_t *parser)
{
	
	return 0;
}

int rFunctionDefinition(Parser_t *parser)
{
	return 0;
}

int rParams(Parser_t *parser)
{
	return 0;
}

int rParam(Parser_t *parser)
{
	return 0;
}

int rParam_n(Parser_t *parser)
{
	return 0;
}

int rType(Parser_t *parser)
{
	return 0;
}

int rStatements(Parser_t *parser)
{
	return 0;
}

int rAssignmentStatement(Parser_t *parser)
{
	return 0;
}

int rConditionalStatement(Parser_t *parser)
{
	return 0;
}

int rWhileLoopStatement(Parser_t *parser)
{
	return 0;
}

int rFunctionCallStatement(Parser_t *parser)
{
	return 0;
}

int rReturnStatement(Parser_t *parser)
{
	return 0;
}

int rAssignment(Parser_t *parser)
{
	return 0;
}

int rArguments(Parser_t *parser)
{
	return 0;
}

int rArgument_n(Parser_t *parser)
{
	return 0;
}

int rReturnValue(Parser_t *parser)
{
	return 0;
}

int rTerm(Parser_t *parser)
{
	return 0;
}