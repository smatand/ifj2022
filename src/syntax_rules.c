/**
 * Project: Translator of language IFJ22
 * @file syntax_rules.c
 * @author Martin Maršalek - xmarsa15
 * @author Andrej Smatana - xsmata03
 * @author János László Vasík - xvasik05
 * 
 * @brief Implementation syntax analysis unit
 */

#include "parser.h"
#include "syntax_rules.h"
#include "parser_macros.h"
#include "error.h"
#include "expr.h"
#include "sym_table_stack.h"
#include "token.h"
#include "dll.h"
#include "generator.h"

int rProgram(Parser_t *parser)
{
	CALL_RULE(rProlog);

	genInit(); // initial code generation

	CALL_RULE(rUnits);
	CALL_RULE(rProgramEnd);

	genEnd();

	return SUCCESS;
}

int rProlog(Parser_t *parser)
{
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_PROLOGUE);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_DECLARE_STRICT);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON);
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
	int retVal = SUCCESS;
	CURRENT_TOKEN_KWORD_GETNEXT(KW_FUNCTION);
	CURRENT_TOKEN_TYPE(TOK_FUNCTION);

	parser->definedFunc = NULL;

	if (htab_find(parser->globalSymTable, parser->currentToken->attribute.strVal->str) != NULL) // redefinition of a function
	{
		// fprintf(stderr, "[ERROR] Semantic error, function redefinition.\n");
		return ERR_SEM;
	}

	token_data_t *data = createTokenDataFunction();
	parser->definedFunc = htab_add(parser->globalSymTable, parser->currentToken->attribute.strVal->str, data);

	genFunctionLabel(parser->definedFunc->key); // generate function label

	// next codegen happens in param and param_n
	parser->onParam = 0; // to keep track of the number of parameters, this is the only time onParam is used

	CALL_FUN(getNextToken);
	
	if (push_empty(parser->localSymStack) != SUCCESS) // push new sym_table
	{
		return ERR_INTERNAL;
	}

	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_PAREN);
	CALL_RULE(rParams); // params are added to the new symtable and generated as variables here
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_PAREN);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_COLON);

	CALL_RULE(rType);
	genFunctionRetType(parser->currentToken->attribute.kwVal);

	CALL_FUN(getNextToken);

	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_BRACE);

	CALL_RULE(rStatements);

	genFunctionEnd(parser->definedFunc->key);
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
	int retVal;
	parser->onParamType = 0;
	CALL_RULE(rType);
	genFunctionParamType(parser->currentToken->attribute.kwVal, parser->onParamType);
	parser->onParamType++;

	CALL_FUN(getNextToken);

	CURRENT_TOKEN_TYPE(TOK_VARIABLE);

	token_data_t *data = createTokenDataVariable();

	htab_add(top(parser->localSymStack), parser->currentToken->attribute.strVal->str, data);
	functionAddParam(parser->definedFunc->data, parser->currentToken->attribute.strVal->str); // add param name to symtable

	// [param_count-1] because inside functionAddParam() it is incremented already
    printf("sub LF@%%_countArgs LF@%%_countArgs int@1\n");
	printf("defvar LF@%%%s%%\n", parser->definedFunc->data->param_IDs[parser->definedFunc->data->param_count-1]);
	genFunctionAmountOfGivenArgsCheck(0);
	printf("pops LF@%%%s%%\n", parser->definedFunc->data->param_IDs[parser->definedFunc->data->param_count-1]);
	genTypeCheck(parser->onParamType, parser->currentToken->attribute.strVal->str);
    printf("sub LF@%%_countArgs LF@%%_countArgs int@1\n");
	//genFunctionAmountOfGivenArgsCheck(parser->onParam, 0);
	fflush(stdout);
	parser->onParam++;

	CALL_FUN(getNextToken);
	return SUCCESS;
}

int rParams_n(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_RIGHT_PAREN)
	{	  // there are no more parameters, return
		;
		//genFunctionAmountOfParamsCheck(parser->onParam);
		//while (parser->onParamType)
		//{
		//	genTypeCheck(--parser->onParamType);
		//}
		//genFunctionAmountOfGivenArgsCheck(0);
	}
	else
	{
		CALL_RULE(rParam_n); // checking next parameter
		CALL_RULE(rParams_n);
	}


	// TODO this needs to be transformed into a %$var% name, not the LF@param
	//while (parser->onParam)
	//{
	//	printf("pops LF@param%d\n", --parser->onParam); // assign argX to paramX
	//}



	return SUCCESS;
}

int rParam_n(Parser_t *parser)
{
	int retVal = SUCCESS;
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_COMMA);

	CALL_RULE(rType);
	genFunctionParamType(parser->currentToken->attribute.kwVal, parser->onParamType);
	parser->onParamType++;

	CALL_FUN(getNextToken);

	CURRENT_TOKEN_TYPE(TOK_VARIABLE);

	token_data_t *data = createTokenDataVariable();

	htab_add(top(parser->localSymStack), parser->currentToken->attribute.strVal->str, data);
	functionAddParam(parser->definedFunc->data, parser->currentToken->attribute.strVal->str); // add param name to symtable

    printf("sub LF@%%_countArgs LF@%%_countArgs int@1\n");
	printf("defvar LF@%%%s%%\n", parser->definedFunc->data->param_IDs[parser->definedFunc->data->param_count-1]);
	genFunctionAmountOfGivenArgsCheck(0);
	printf("pops LF@%%%s%%\n", parser->definedFunc->data->param_IDs[parser->definedFunc->data->param_count-1]);

	genTypeCheck(parser->onParamType, parser->currentToken->attribute.strVal->str);
	//genFunctionAmountOfGivenArgsCheck(0);

	fflush(stdout);
	parser->onParam++;
	// TODO: check types of arguments and their existence somehow, in rType maybe? helper vars that store types maybe?

	CALL_FUN(getNextToken);
	return SUCCESS;
}

int rType(Parser_t *parser)
{
	// the token in this position must either be type_id (checked by the scanner)
	// or a keyword (which we have to check ourselves)
	if (parser->currentToken->type == TOK_TYPE_ID)
	{
		;
	}
	else if (parser->currentToken->type == TOK_KEYWORD)
	{
		switch (parser->currentToken->attribute.kwVal)
		{
		case KW_INT:
		case KW_FLOAT:
		case KW_STRING:
		case KW_VOID:
			break;
		}
	}
	else
	{ // incorrect token, syn err
		return ERR_SYN_ANALYSIS;
	}

	return SUCCESS;
}

int rStatements(Parser_t *parser)
{
	int retVal = SUCCESS; // SUCCESS or ERR_*
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
		int retToken; // last read non valid token in expression 

		if ((retVal = exprParse(parser->currentToken, parser->nextToken, &retToken, parser)) != SUCCESS)
		{
			return retVal;
		}
		parser->nextToken->type = retToken;
		CALL_FUN(getNextToken);			   			// ensuring continuity of tokens after returning from bottom up
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON); 	// expressions in statements end with a semicolon
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
	int retVal = SUCCESS; // SUCCESS or ERR_*
	if (checkTokenType(parser->nextToken, TOK_ASSIGN) == 0)
	{
		CALL_RULE(rAssignmentStatement);
	}
	else if (checkForOperator(parser->nextToken) == 0)
	{
		int retToken; // last read non valid token in expression 
		
		if ((retVal = exprParse(parser->currentToken, parser->nextToken, &retToken, parser)) != SUCCESS)
		{
			return retVal;
		}
		parser->nextToken->type = retToken;
		CALL_FUN(getNextToken);		// ensuring continuity of tokens after bottom up
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON);
	}
	else if (checkTokenType(parser->nextToken, TOK_SEMICOLON) == 0)
	{
		// "$foo;" is a valid statement, though it does nothing
		if (htab_find(top(parser->localSymStack), parser->currentToken->attribute.strVal->str) == NULL)
		{
			fprintf(stderr, "[ERROR] Semantic error, refrencing undefined variable.\n");
			exit(ERR_SEM_UNDEFINED_VAR);
		}
		// skipping two tokens
		CALL_FUN(getNextToken);
		CALL_FUN(getNextToken);
	}
	else
	{
		return ERR_SYN_ANALYSIS;
	}
	return SUCCESS;
}

int rAssignmentStatement(Parser_t *parser)
{
	int retVal = SUCCESS; // SUCCESS or ERR_*

	CURRENT_TOKEN_TYPE(TOK_VARIABLE); // TODO: codegen
	htab_pair_t *definedVar = NULL;

	if ((definedVar = htab_find((top(parser->localSymStack)), parser->currentToken->attribute.strVal->str)) == NULL) // variable hasn't been declared yet
	{
		token_data_t *data = createTokenDataVariable();

		definedVar = htab_add(top(parser->localSymStack), parser->currentToken->attribute.strVal->str, data);

		printf("defvar LF@%s\n", definedVar->key); // generate defvar code
	}

	// TODO: set or change the value and type(?) of the declared variable (code generation)

	CALL_FUN(getNextToken);

	CURRENT_TOKEN_TYPE_GETNEXT(TOK_ASSIGN);

	if (checkTokenType(parser->currentToken, TOK_FUNCTION)==0)
	{
		CALL_RULE(rFunctionCallStatement);
		printf("move LF@%s TF@%%retval\n", definedVar->key); // move return value from func (now in TF) to var
	}
	else
	{
		int retToken; // last read non valid token in expression 

		if ((retVal = exprParse(parser->currentToken, NULL, &retToken, parser)) != SUCCESS)
		{
			return retVal;
		}

		printf("pops LF@%s\n", definedVar->key); // pop stack into new value

		parser->nextToken->type = retToken;
		CALL_FUN(getNextToken); // ensuring continuity of tokens after returning from bottom up
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON);
	}
	return SUCCESS;
}

int rConditionalStatement(Parser_t *parser)
{
	int retVal = SUCCESS; // SUCCESS or ERR_*

	CURRENT_TOKEN_KWORD_GETNEXT(KW_IF);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_PAREN);

	int retToken; // last read non valid token in expression 

	if ((retVal = exprParse(parser->currentToken, parser->nextToken, &retToken, parser)) != SUCCESS)
	{
		return retVal;
	}
	parser->nextToken->type = retToken;

	printf("pushs bool@false\n");					// helper variable for if condition
	printf("jumpifeqs _if%d\n", parser->ifCounter); // skip code if expr result was false

	CALL_FUN(getNextToken); // ensuring continuity of tokens after returning from bottom up
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_PAREN);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_BRACE);
	CALL_RULE(rStatements);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_BRACE);
	CURRENT_TOKEN_KWORD_GETNEXT(KW_ELSE);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_BRACE);
	CALL_RULE(rStatements);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_BRACE);

	printf("label _if%d\n", parser->ifCounter); // label for skipping code
	fflush(stdout);

	parser->ifCounter++;
	return SUCCESS;
}

int rWhileLoopStatement(Parser_t *parser)
{
	int retVal = SUCCESS; // SUCCESS or ERR_*

	CURRENT_TOKEN_KWORD_GETNEXT(KW_WHILE);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_PAREN);

	printf("label _while_begin%d\n", parser->whileCounter); // label for jumping backwards TODO: does this work?

	int retToken; // last read non valid token in expression 

	if ((retVal = exprParse(parser->currentToken, parser->nextToken, &retToken, parser)) != SUCCESS)
	{
		return retVal;
	}
	parser->nextToken->type = retToken;

	printf("pushs bool@false\n");							  // helper variable for while condition
	printf("jumpifeqs _while_end%d\n", parser->whileCounter); // skip code if expr result was false

	CALL_FUN(getNextToken); // ensuring continuity of tokens after returning from bottom up
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_PAREN);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_BRACE);

	CALL_RULE(rStatements);
	CURRENT_TOKEN_TYPE_GETNEXT(TOK_RIGHT_BRACE);

	printf("jump _while_begin%d\n", parser->whileCounter); // label for jumping backwards
	printf("label _while_end%d\n", parser->whileCounter);  // label for skipping code
	fflush(stdout);

	parser->whileCounter++;
	return SUCCESS;
}

int rFunctionCallStatement(Parser_t *parser)
{
	CURRENT_TOKEN_TYPE(TOK_FUNCTION);

	if (htab_find(parser->globalSymTable, parser->currentToken->attribute.strVal->str) == NULL)
	{
		return ERR_SEM;
	}

	printf("createframe\n");

	htab_pair_t *calledFunction = htab_find(parser->globalSymTable, parser->currentToken->attribute.strVal->str);

	// if (!strcmp(calledFunction->key, "write")) {
	//	printf("write ");
	// }

	CALL_FUN(getNextToken);

	parser->onArg = 0;

	CURRENT_TOKEN_TYPE_GETNEXT(TOK_LEFT_PAREN);
	CALL_RULE(rArguments);

	printf("call $%s\n", calledFunction->key);

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

	DLLPrintAllReversed(parser->codeGen); // prints given arguments
	printf("pushs int@%d\n", parser->onArg); // prints count of arguments
	DLLDispose(parser->codeGen); 

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
		fprintf(stderr, "[ERROR] Syntax error in rArgument_n\n");
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
	int retVal = SUCCESS; // SUCCESS or ERR_*

	if (parser->currentToken->type == TOK_SEMICOLON)
	{
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON); // TODO: missing return value -> check if void function?
	}
	else
	{

		int retToken; // last read non valid token in expression 

		if ((retVal = exprParse(parser->currentToken, parser->nextToken, &retToken, parser)) != SUCCESS)
		{
			return retVal;
		}

		printf("pops LF@%%retval\n"); // TODO: check some stuff here maybe (void?, return type?), maybe in assignment instead?

		parser->nextToken->type = retToken;
		CALL_FUN(getNextToken); // ensuring continuity of tokens after returning from bottom up
		CURRENT_TOKEN_TYPE_GETNEXT(TOK_SEMICOLON);
	}
	return retVal;
}

int rTerm(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_VARIABLE)
	{
		if (htab_find(top(parser->localSymStack), parser->currentToken->attribute.strVal->str) == NULL) // variable not declared
		{
			fprintf(stderr, "[ERROR] Semantic error, passing undeclared variable as argument.\n");
			exit(ERR_SEM_UNDEFINED_VAR);
		}

		// todo here do some shitcodegen to generate params with
		char * varName = malloc(parser->currentToken->attribute.strVal->realLen + 5); // 5 for %% %%
		if (varName == NULL)
		{
			fprintf(stderr, "[ERROR] Memory allocation error.\n");
			exit(ERR_INTERNAL);
		}

		snprintf(varName, parser->currentToken->attribute.strVal->realLen+5, "%%%s%%", parser->currentToken->attribute.strVal->str);
		CODEGEN_INSERT_IN_DLL("pushs LF@", varName);

		fflush(stdout);
		free(varName);
	}
	else if (parser->currentToken->type == TOK_INT_LIT)
	{
		// TODO: check for type matching in code gen
		// printf("defvar TF@%%%d\n", parser->onArg);
		// printf("move TF@%%%d int@%d\n", parser->onArg, parser->currentToken->attribute.intVal); // TODO: no conversion necessary here?
		char *str = convertIntToIFJ(parser->currentToken->attribute.intVal);
		if (str == NULL)
		{
			return ERR_INTERNAL;
		}

		CODEGEN_INSERT_IN_DLL("pushs int@", str);

		fflush(stdout);
		free(str);
	}
	else if (parser->currentToken->type == TOK_STRING_LIT)
	{

		// TODO: check for type matching in code gen
		// printf("defvar TF@%%%d\n", parser->onArg);

		char *str = convertStringToIFJ(parser->currentToken->attribute.strVal->str); // TODO: does this work?
		if (str == NULL)
		{
			return ERR_INTERNAL;
		}
		// printf("move TF@%%%d string@%s\n", parser->onArg, temp);

		CODEGEN_INSERT_IN_DLL("pushs string@", str);

		fflush(stdout);
		free(str);
	}
	else if (parser->currentToken->type == TOK_DEC_LIT)
	{
		// TODO: check for type matching in code gen
		// printf("defvar TF@%%%d\n", parser->onArg);
		// printf("move TF@%%%d float@%a\n", parser->onArg, parser->currentToken->attribute.decVal); // TODO: does this work?
		char *str = convertFloatToIFJ(parser->currentToken->attribute.decVal);
		if (str == NULL)
		{
			return ERR_INTERNAL;
		}

		CODEGEN_INSERT_IN_DLL("pushs float@", str);
		fflush(stdout);
		free(str);
	}
	else if (parser->currentToken->type == TOK_KEYWORD && parser->currentToken->attribute.kwVal == KW_NULL)
	{
		printf("pushs nil@nil\n");
		fflush(stdout);
	}
	else
	{
		fprintf(stderr, "[ERROR] Syntax error in rTerm\n");
		return ERR_SYN_ANALYSIS;
	}

	parser->onArg++;
	CALL_FUN(getNextToken);
	
	return SUCCESS;
}

int rProgramEnd(Parser_t *parser)
{
	if (parser->currentToken->type == TOK_END_PROLOGUE || parser->currentToken->type == TOK_EOF)
	{
		return SUCCESS;
	}
}
