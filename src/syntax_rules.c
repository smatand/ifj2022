/**
 * @file syntax_rules.h
 *
 * @brief Implementation of syntax rules for IFJ22
 */

#include "parser.h"
#include "parser_macros.h"
#include "error.h"

int rProgram(Parser_t *parser)
{
	CALL_RULE(rProlog);
	CALL_RULE(rUnits);
	CALL_RULE(rProgramEnd);
	return SUCCESS;
}

int rProlog(Parser_t *parser)
{
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_PROLOGUE);
	return SUCCESS;
}

int rUnits(Parser_t *parser)
{
	if(parser->currentToken->type == TOK_EOF || parser->currentToken->type == TOK_END_PROLOGUE)
	{ // got to the end, return SUCCESS
		;
	}
	else
	{ // there are more units, continue
		CALL_RULE(rUnit);
		CALL_RULE(rUnits);
	}
	return SUCCESS;
}

int rUnit(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_KEYWORD && parser->currentToken->attribute.kwVal == KW_FUNCTION)
	{
		CALL_RULE(rFunctionDefinition);
	}
	else
	{
		CALL_RULE(rStatements);
	}
	return 0;
}

int rFunctionDefinition(Parser_t *parser)
{
	// 7. <function_definition> ->  "function" ID "(" <params> ")" ":" <type> "{" <statements> "}"
	CURRENT_TOKEN_KWORD_GETNEXT(KW_FUNCTION);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_IDENTIFIER); // TODO: TOK_IDENTIFIER can be a function or a variable, check if it's a function
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_PAREN);
	CALL_RULE(rParams);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_PAREN);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_COLON);
	CALL_RULE(rType);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_BRACE);
	// TODO: EOL? probably not...
	CALL_RULE(rStatements);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_BRACE);
	return 0;
}

int rParams(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_TYPE_ID)
	{
		CALL_RULE(rParam);
		CALL_RULE(rParam_n);
		return 0;
	}
	else // epsilon
	{
		return 0;
	}
}

int rParam(Parser_t *parser)
{
	CALL_RULE(rType);
	return 0;
}

int rParam_n(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_COMMA)
	{
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_COMMA);
		CALL_RULE(rType);
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_IDENTIFIER);
		CALL_RULE(rParam_n);
	}
	else // epsilon
	{
		return 0;
	}
}

int rType(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_TYPE_ID)
	{
		// TODO: SEMANTIC EFFECT
	}
	else
	{
		return 1;
	}
}

int rStatements(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_IDENTIFIER)
	{
		// TODO: CHECK IF ITS A FUNCTION OR A VARIABLE
		CALL_RULE(rStatements);
	}
	else if (parser->currentToken->type == TOK_KEYWORD && parser->currentToken->attribute.kwVal == KW_IF)
	{
		CALL_RULE(rConditionalStatement);
		CALL_RULE(rStatements);
	}
	else if (parser->currentToken->type == TOK_KEYWORD && parser->currentToken->attribute.kwVal == KW_WHILE)
	{
		CALL_RULE(rWhileLoopStatement);
		CALL_RULE(rStatements);
	}
	else if (parser->currentToken->type == TOK_KEYWORD && parser->currentToken->attribute.kwVal == KW_RETURN)
	{
		CALL_RULE(rReturnStatement);
		CALL_RULE(rStatements);
	}
	else // epsilon
	{
		return 0;
	}
}

int rAssignmentStatement(Parser_t *parser)
{
	// 23. <assignment_statement>  ->  "$" ID "=" <assignment> ";"

	CURRENT_TOKEN_TYPE_GETNEXT(TOK_IDENTIFIER);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_ASSIGN);
	CALL_RULE(rAssignment);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON);
	return 0;
}

int rConditionalStatement(Parser_t *parser)
{
	// 24. <conditional_statement>  ->  "if" "(" <expression> ")" "{" <statements> "}" "else" "{" <statements> "}"

	CURRENT_TOKEN_KWORD_GETNEXT(KW_IF);
	CURRENT_TOKEN_TYPE(TOK_LEFT_PAREN); // leave the parenthesis for the expression parser
	// EXPRESSION!
	// TODO: right parenthesis may be left idk... CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_PAREN);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_BRACE);
	CALL_RULE(rStatements);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_BRACE);
	CURRENT_TOKEN_KWORD_GETNEXT(KW_ELSE);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_BRACE);
	CALL_RULE(rStatements);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_BRACE);
	return 0;
}

int rWhileLoopStatement(Parser_t *parser)
{
	// 25. <while_loop_statement>  ->  "while" "(" <expression> ")" "{" <statements> "}"

	CURRENT_TOKEN_KWORD_GETNEXT(KW_WHILE);
	CURRENT_TOKEN_TYPE(TOK_LEFT_PAREN); // leave the parenthesis for the expression parser
	// EXPRESSION!
	// TODO: right parenthesis may be left idk... CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_PAREN);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_BRACE);
	CALL_RULE(rStatements);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_BRACE);
	return 0;
}

int rFunctionCallStatement(Parser_t *parser)
{
	// 26. <function_call_statement>  ->  ID "(" <arguments> ")" ";"

	CURRENT_TOKEN_TYPE_GETNEXT(TOK_IDENTIFIER);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_PAREN);
	CALL_RULE(rArguments);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_PAREN);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON);
	return 0;
}

int rReturnStatement(Parser_t *parser)
{
	// 27. <return_statement>  ->  "return" <return_value> ";"

	CURRENT_TOKEN_KWORD_GETNEXT(KW_RETURN);
	CALL_RULE(rReturnValue);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON);
	return 0;
}

int rAssignment(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_IDENTIFIER && parser->nextToken->type == TOK_LEFT_PAREN)
	{
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_IDENTIFIER);
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_PAREN);
		CALL_RULE(rArguments);
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_PAREN);
	}
	else
	{
		// EXPRESSION!
	}
	return 0;
}

int rArguments(Parser_t *parser)
{
	rTerm(parser);
	return 0;
}

int rArgument_n(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_COMMA)
	{
		CALL_RULE(rTerm);
		CALL_RULE(rArgument_n);
	}
	else // epsilon
	{
		return 0;
	}
}

int rReturnValue(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_SEMICOLON)
	{
		return 0;
	}
	else
	{
		// EXPRESSION!
	}
	return 0; // ?
}

int rTerm(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_IDENTIFIER)
	{
		// TODO: SEMANTIC EFFECT?
	}
	else if (parser->currentToken->type == TOK_INT_LIT)
	{
		// TODO: SEMANTIC EFFECT?
	}
	else if (parser->currentToken->type == TOK_STRING_LIT)
	{
		// TODO: SEMANTIC EFFECT?
	}
	else if (parser->currentToken->type == TOK_DEC_LIT)
	{
		// TODO: SEMANTIC EFFECT?
	}
	else if (parser->currentToken->type == TOK_KEYWORD && parser->currentToken->attribute.kwVal == KW_NULL)
	{
		// TODO: SEMANTIC EFFECT?
	}
	else
	{
		return 1;
	}
	return 0;
}

int rProgramEnd(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_END_PROLOGUE)
	{
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_END_PROLOGUE);
	}
	CURRENT_TOKEN_TYPE(TOK_EOF);
	return 0;
}