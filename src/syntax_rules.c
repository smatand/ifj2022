/**
 * @file syntax_rules.h
 *
 * @brief Implementation of syntax rules for IFJ22
 */

#include "parser.h"
#include "parser_macros.h"
#include "error.h"
#include "expr.h"

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
	// TODO generate code?
	return SUCCESS;
}

int rUnits(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_EOF || parser->currentToken->type == TOK_END_PROLOGUE)
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
	return SUCCESS;
}

int rFunctionDefinition(Parser_t *parser)
{
	CURRENT_TOKEN_KWORD_GETNEXT(KW_FUNCTION);
	CURRENT_TOKEN_TYPE(TOK_FUNCTION);

	if (parser->firstPass == true)
	{
		if (htab_find(parser->globalSymTable, parser->currentToken->attribute.strVal->str) != NULL) // redefinition of a function
		{
			fprintf(stderr, "[ERROR] Semantic error, function redefinition.\n");
			exit(ERR_SEM);
		}
		char *ID = createTokenKey(parser->currentToken->attribute.strVal, TOKTYPE_FUNCTION);

		param_list_t *paramList = createParamList(); // malloc is called here
		token_data_t *data = createTokenDataFunction(ID, paramList, DATA_UNDEFINED, true);

		parser->latestFuncDeclared = htab_add(parser->globalSymTable, ID, data);
	}

	getNextToken(parser);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_PAREN);
	CALL_RULE(rParams); // function parameters are added to the sym_table here
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_PAREN);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_COLON);

	if (parser->firstPass == true)
		parser->latestFuncDeclared->data->data.function.paramsFilled = true;

	CALL_RULE(rType);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_BRACE);

	if (parser->firstPass == false)
		push_empty(parser->localSymStack); // push new sym_table

	CALL_RULE(rStatements);

	if (parser->firstPass == false)
		pop(parser->localSymStack); // pop new sym_table

	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_BRACE);
	return 0;
}

int rParams(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_RIGHT_PAREN)
	{ // there are no parameters, return
		;
	}
	else
	{
		CALL_RULE(rParam);	  // one parameter
		CALL_RULE(rParams_n); // 1+ parameters
	}
	return SUCCESS;
}

int rParam(Parser_t *parser)
{
	CALL_RULE(rType);

	if (parser->firstPass == true) // add param to param_list of the declared function
	{
		// current token is still the type token
		param_list_t *params = htab_find(parser->globalSymTable, parser->latestFuncDeclared)->data->data.function.param_list;
		data_type_t type;

		switch (parser->currentToken->attribute.kwVal)
		{
		case KW_INT:
			type = DATA_INT;
			break;
		case KW_FLOAT:
			type = DATA_FLOAT;
			break;
		case KW_STRING:
			type = DATA_STRING;
			break;
		default:
			fprintf(stderr, "[ERROR] Error in rParam switch.\n");
			exit(ERR_INTERNAL);
		}
		// TODO: what about null? also this switch should really be a function

		getNextToken(parser);
		CURRENT_TOKEN_TYPE(TOK_VARIABLE);

		addToParamList(params, type, parser->currentToken->attribute.strVal->str); // malloc is called here, for the name string
	}

	getNextToken(parser);
	CURRENT_TOKEN_TYPE(TOK_VARIABLE);

	getNextToken(parser); // TODO: add variable to symtable and generate code (?)
	return SUCCESS;
}

int rParams_n(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_RIGHT_PAREN)
	{	  // there are no more parameters, return
		; // maybe export parameters at this point all ot once, or one by one on the places where i commented
	}
	else
	{
		CALL_RULE(rParam_n); // checking next parameter
		CALL_RULE(rParams_n);
	}
	return SUCCESS;
}

int rParam_n(Parser_t *parser)
{
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_COMMA);

	CALL_RULE(rType);

	if (parser->firstPass == true) // add param to param_list of the declared function
	{
		// current token is still the type token
		param_list_t *params = parser->latestFuncDeclared->data->data.function.param_list;
		data_type_t type;

		switch (parser->currentToken->attribute.kwVal)
		{
		case KW_INT:
			type = DATA_INT;
			break;
		case KW_FLOAT:
			type = DATA_FLOAT;
			break;
		case KW_STRING:
			type = DATA_STRING;
			break;
		default:
			fprintf(stderr, "[ERROR] Error in rParam switch.\n");
			exit(ERR_INTERNAL);
		}
		// TODO: what about null? also this switch should really be a function

		getNextToken(parser);
		CURRENT_TOKEN_TYPE(TOK_VARIABLE);

		addToParamList(params, type, parser->currentToken->attribute.strVal->str); // malloc is called here, for the name string
	}

	getNextToken(parser);
	CURRENT_TOKEN_TYPE(TOK_VARIABLE);

	getNextToken(parser); // TODO: add variable to symtable and generate code (?)
	return SUCCESS;
}

int rType(Parser_t *parser)
{
	// the token in this position must either be type_id (checked by the scanner)
	// or a keyword (which we have to check ourselves)
	if (parser->currentToken->type == TOK_TYPE_ID || (parser->currentToken->type == TOK_KEYWORD &&
													  (parser->currentToken->attribute.kwVal == KW_INT || parser->currentToken->attribute.kwVal == KW_FLOAT ||
													   parser->currentToken->attribute.kwVal == KW_STRING)))
	{
		if (parser->firstPass == true && parser->latestFuncDeclared->data->data.function.paramsFilled == true)
		{
			// if the function being declared already has all its parameters, then this rule is called for the return type, so we add it to the sym_table function data
			data_type_t type;

			switch (parser->currentToken->attribute.kwVal)
			{
			case KW_INT:
				type = DATA_INT;
				break;
			case KW_FLOAT:
				type = DATA_FLOAT;
				break;
			case KW_STRING:
				type = DATA_STRING;
				break;
			default:
				fprintf(stderr, "[ERROR] Error in rType switch.\n");
				exit(ERR_INTERNAL);
			}
			// TODO: what about null? also this switch should really be a function

			parser->latestFuncDeclared->data->data.function.return_type = type;
		}
		return SUCCESS;
	}
	else
	{ // incorrect token, syn err
		return ERR_SYN_ANALYSIS;
	}
}

int rStatements(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_FUNCTION)
	{
		CALL_RULE(rFunctionCallStatement); // we check whether the function is defined in rFunctionCallStatement
		CALL_RULE(rStatements);
	}
	else if (parser->currentToken->type == TOK_VARIABLE)
	{
		CALL_RULE(rVariableStatement); // we check whether the variable is defined in rVariableStatement
		CALL_RULE(rStatements);
	}
	else if (parser->currentToken->type == TOK_INT_LIT || parser->currentToken->type == TOK_DEC_LIT ||
			 parser->currentToken->type == TOK_STRING_LIT)
	{
		int *ret;
		int retVal = exprParse(parser->currentToken, parser->nextToken, ret);
		if (retVal != SUCCESS)
		{
			destroyParser(parser);
			return retVal;
		}
		parser->nextToken->type = ret;
		getNextToken(parser);					   // ensuring continuity of tokens after returning from bottom up
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON); // expressions in statements end with a semicolon
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
	else
	{
		; // epsilon transition
	}
	return SUCCESS;
}

int rVariableStatement(Parser_t *parser)
{
	if (parser->firstPass == false && htab_find(top(parser->localSymStack), parser->currentToken->attribute.strVal->str) == NULL)
	{
		fprintf(stderr, "[ERROR] Semantic error, refrencing undefined variable.\n");
		exit(ERR_SEM_UNDEFINED_VAR);
	}

	if (checkTokenType(parser->nextToken, TOK_ASSIGN))
	{
		CALL_RULE(rAssignmentStatement);
	}
	else if (checkForOperator(parser->nextToken) == 0)
	{
		int *ret;
		int retVal = exprParse(parser->currentToken, parser->nextToken, ret);
		if (retVal != SUCCESS)
		{
			destroyParser(parser);
			return retVal;
		}
		parser->nextToken->type = ret;
		getNextToken(parser); // ensuring continuity of tokens after returning from bottom up
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON);
	}
	else if (checkTokenType(parser->nextToken, TOK_SEMICOLON))
	{						  // "$foo;" is a valid statement, though it does nothing
		getNextToken(parser); // skipping two tokens
		getNextToken(parser);
	}
	else
	{
		return ERR_SYN_ANALYSIS;
	}
	return SUCCESS;
}

int rAssignmentStatement(Parser_t *parser)
{
	CURRENT_TOKEN_TYPE(TOK_VARIABLE); // TODO check if defined, codegen

	if (parser->firstPass == false && (top(parser->localSymStack), parser->currentToken->attribute.strVal->str) == NULL) // variable hasn't been declared yet
	{
		char *ID = createTokenKey(parser->currentToken->attribute.strVal, TOKTYPE_FUNCTION); // malloc is called here
		token_data_t *data = createTokenDataVariable(ID, DATA_UNDEFINED);					 // TODO: assign data type based on value

		htab_add(top(parser->localSymStack), ID, data);
	}

	// TODO: set or change the value and type(?) of the declared variable (code generation)

	getNextToken(parser);

	CURRENT_TOKEN_TYPE_GETNEXT(TOK_ASSIGN);

	if (checkTokenType(parser->currentToken, TOK_FUNCTION))
	{
		CALL_RULE(rFunctionCallStatement);
	}
	else
	{
		int *ret;
		int retVal = exprParse(parser->currentToken, NULL, ret);
		if (retVal != SUCCESS)
		{
			destroyParser(parser);
			return retVal;
		}
		parser->nextToken->type = ret;
		getNextToken(parser); // ensuring continuity of tokens after returning from bottom up
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON);
	}
	return SUCCESS;
}

int rConditionalStatement(Parser_t *parser)
{
	CURRENT_TOKEN_KWORD_GETNEXT(KW_IF);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_PAREN);
	int *ret;
	int retVal = exprParse(parser->currentToken, parser->nextToken, ret);
	if (retVal != SUCCESS)
	{
		destroyParser(parser);
		return retVal;
	}
	parser->nextToken->type = ret;
	getNextToken(parser); // ensuring continuity of tokens after returning from bottom up
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_PAREN);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_BRACE);
	CALL_RULE(rStatements);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_BRACE);
	CURRENT_TOKEN_KWORD_GETNEXT(KW_ELSE);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_BRACE);
	CALL_RULE(rStatements);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_BRACE);
	return SUCCESS;
}

int rWhileLoopStatement(Parser_t *parser)
{
	CURRENT_TOKEN_KWORD_GETNEXT(KW_WHILE);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_PAREN);
	int *ret;
	int retVal = exprParse(parser->currentToken, parser->nextToken, ret);
	if (retVal != SUCCESS)
	{
		destroyParser(parser);
		return retVal;
	}
	parser->nextToken->type = ret;
	getNextToken(parser); // ensuring continuity of tokens after returning from bottom up
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_PAREN);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_BRACE);
	CALL_RULE(rStatements);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_BRACE);
	return SUCCESS;
}

int rFunctionCallStatement(Parser_t *parser)
{
	CURRENT_TOKEN_TYPE(TOK_FUNCTION);

	if (parser->firstPass == false)
	{
		if ((parser->latestFuncCalled = htab_find(parser->globalSymTable, parser->currentToken->attribute.strVal->str)) == NULL)
		{
			fprintf(stderr, "[ERROR] Semantic error, calling undefined function.\n");
			exit(ERR_SEM);
		}
		else
		{
			parser->currentArgument = 0; // ready to start checking arguments
			// pointer to the currently called function is in parser->latestFuncCalled
		}
	}

	getNextToken(parser);

	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_PAREN);
	CALL_RULE(rArguments); // arguments are checked here, one by one

	if (parser->firstPass == false)
	{
		if (parser->currentArgument != parser->latestFuncCalled->data->data.function.param_list->size) // argument count check
		{
			fprintf(stderr, "[ERROR] Semantic error, too few arguments in function call.\n");
			exit(ERR_SEM_PARAMS);
		}
		else
		{
			parser->currentArgument = 0; // reset argument counter
			// TODO: parser->latestFuncCalled = NULL; ?, probably unnecessary
		}
	}

	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_PAREN);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON);
	return SUCCESS;
}

int rReturnStatement(Parser_t *parser)
{
	CURRENT_TOKEN_KWORD_GETNEXT(KW_RETURN);
	CALL_RULE(rReturnValue);
	return SUCCESS;
}

int rArguments(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_RIGHT_PAREN)
	{
		; // epsilon
	}
	else
	{
		CALL_RULE(rTerm);
		CALL_RULE(rArguments_n);
	}
	return SUCCESS;
}

int rArgument_n(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_COMMA)
	{
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_COMMA);
		CALL_RULE(rTerm);
	}
	else
	{
		return ERR_SYN_ANALYSIS;
	}
	return SUCCESS;
}

int rArguments_n(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_RIGHT_PAREN)
	{
		; // epsilon
	}
	else
	{
		CALL_RULE(rArgument_n);
		CALL_RULE(rArguments_n);
	}
	return SUCCESS;
}

int rReturnValue(Parser_t *parser)
{
	int retVal = SUCCESS;
	if (parser->currentToken->type == TOK_SEMICOLON)
	{
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON); // missing return value -> check if void function?
	}
	else
	{
		int *ret;
		int retVal = exprParse(parser->currentToken, parser->nextToken, ret);
		if (retVal != SUCCESS)
		{
			destroyParser(parser);
			return retVal;
		}
		parser->nextToken->type = ret;
		getNextToken(parser); // ensuring continuity of tokens after returning from bottom up
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON);
	}
	return retVal;
}

int rTerm(Parser_t *parser)
{
	if (parser->firstPass == false && (parser->currentArgument >= parser->latestFuncCalled->data->data.function.param_list->size))
	{
		fprintf(stderr, "[ERROR] Semantic error, too many arguments in function call.\n");
		exit(ERR_SEM_PARAMS);
	}

	if (parser->currentToken->type == TOK_VARIABLE)
	{
		if (parser->firstPass == false)
		{
			if (htab_find(top(parser->localSymStack), parser->currentToken->attribute.strVal->str) == NULL) // variable not declared
			{
				fprintf(stderr, "[ERROR] Semantic error, passing undeclared variable as argument.\n");
				exit(ERR_SEM_UNDEFINED_VAR);
			}

			// check if the variable type matches the parameter type
			data_type_t type;

			switch (parser->currentToken->attribute.kwVal)
			{
			case KW_INT:
				type = DATA_INT;
				break;
			case KW_FLOAT:
				type = DATA_FLOAT;
				break;
			case KW_STRING:
				type = DATA_STRING;
				break;
			default:
				fprintf(stderr, "[ERROR] Error in rTerm switch.\n");
				exit(ERR_INTERNAL);
			}
			// TODO: what about null? also this switch should really be a function

			if (parser->latestFuncCalled->data->data.function.param_list->vector[parser->currentArgument].type != type)
			{
				fprintf(stderr, "[ERROR] Semantic error, argument and parameter types don't match.\n");
				exit(ERR_SEM_PARAMS);
			}
			else
			{
				parser->currentArgument++;
			}
		}
	}
	else if (parser->currentToken->type == TOK_INT_LIT)
	{
		if (parser->firstPass == false)
		{
			if (parser->latestFuncCalled->data->data.function.param_list->vector[parser->currentArgument].type != DATA_INT)
			{
				fprintf(stderr, "[ERROR] Semantic error, argument and parameter types don't match.\n");
				exit(ERR_SEM_PARAMS);
			}
			else
			{
				parser->currentArgument++;
			}
		}
	}
	else if (parser->currentToken->type == TOK_STRING_LIT)
	{
		if (parser->firstPass == false)
		{
			if (parser->latestFuncCalled->data->data.function.param_list->vector[parser->currentArgument].type != DATA_STRING)
			{
				fprintf(stderr, "[ERROR] Semantic error, argument and parameter types don't match.\n");
				exit(ERR_SEM_PARAMS);
			}
			else
			{
				parser->currentArgument++;
			}
		}
	}
	else if (parser->currentToken->type == TOK_DEC_LIT)
	{
		if (parser->firstPass == false)
		{
			if (parser->latestFuncCalled->data->data.function.param_list->vector[parser->currentArgument].type != DATA_FLOAT)
			{
				fprintf(stderr, "[ERROR] Semantic error, argument and parameter types don't match.\n");
				exit(ERR_SEM_PARAMS);
			}
			else
			{
				parser->currentArgument++;
			}
		}
	}
	else if (parser->currentToken->type == TOK_KEYWORD && parser->currentToken->attribute.kwVal == KW_NULL)
	{
		if (parser->firstPass == false)
		{
			if ((parser->firstPass == false) && (parser->latestFuncCalled->data->data.function.param_list->vector[parser->currentArgument].type != DATA_NULL))
			{
				fprintf(stderr, "[ERROR] Semantic error, argument and parameter types don't match.\n");
				exit(ERR_SEM_PARAMS);
			}
			else
			{
				parser->currentArgument++;
			}
		}
	}
	else
	{
		return ERR_SYN_ANALYSIS;
	}
	return SUCCESS;
}

int rProgramEnd(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_END_PROLOGUE)
	{
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_END_PROLOGUE);
	}
	CURRENT_TOKEN_TYPE(TOK_EOF);
	return SUCCESS;
}