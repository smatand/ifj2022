/**
 * @file syntax_rules.h
 *
 * @brief Implementation of syntax rules for IFJ22
 */

#include "parser.h"

int rProgram()
{
	CALL_RULE(rProlog);
	CALL_RULE(rUnits);
	CALL_RULE(rProgramEnd);
	return 0;
}

int rProlog()
{
	checkTokenType(TOK_PROLOGUE);
	return 0;
}

int rUnits()
{
	CALL_RULE(rUnit);
	return 0;
}

int rUnit()
{
	return 0;
}

int rFunctionDefinition()
{
	return 0;
}

int rParams()
{
	return 0;
}

int rParam()
{
	return 0;
}

int rParam_n()
{
	return 0;
}

int rType()
{
	return 0;
}

int rStatements()
{
	return 0;
}

int rAssignmentStatement()
{
	return 0;
}

int rConditionalStatement()
{
	return 0;
}

int rWhileLoopStatement()
{
	return 0;
}

int rFunctionCallStatement()
{
	return 0;
}

int rReturnStatement()
{
	return 0;
}

int rAssignment()
{
	return 0;
}

int rArguments()
{
	return 0;
}

int rArgument_n()
{
	return 0;
}

int rReturnValue()
{
	return 0;
}

int rTerm()
{
	return 0;
}