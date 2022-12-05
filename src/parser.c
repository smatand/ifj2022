/**
 * Project: Translator of language IFJ22
 * @file parser.c
 * @author Andrej Smatana - xsmata03
 * @author János László Vasík - xvasik05
 * @author Martin Maršalek - xmarsa15
 * 
 * @brief Implementation of parser module for IFJ22
 */

#include "parser.h"
#include "error.h"
#include "sym_table_stack.h"

Parser_t *initParser()
{
	Parser_t *parser = malloc(sizeof(Parser_t));

	if (parser == NULL)
	{
		goto returnNull;
	}

	if ((parser->globalSymTable = htab_init(INITIAL_BUCKET_COUNT)) == NULL)
	{
		goto freeParser;
	}

	if ((parser->currentToken = tokenInit()) == NULL) // allocate memory for the first token
	{
		goto freeHtab;
	}

	if ((parser->nextToken = tokenInit()) == NULL) // allocate memory for the second token
	{
		goto freeCurrentToken;
	}
	parser->codeGen = malloc(sizeof(DLList_t));
	if (parser->codeGen == NULL)
	{
		goto freeNextToken;
	}
	DLLInit(parser->codeGen);
	if ((scanToken(parser->currentToken)) != SUCCESS) // scan first token
	{
		goto freeDLList;
	}

	if ((scanToken(parser->nextToken)) != SUCCESS) // scan second token
	{
		goto freeDLList;
	}

	if ((init_stack(&(parser->localSymStack))) != SUCCESS)
	{
		goto freeDLList;
	}

	push_empty(parser->localSymStack);

	// built-in functions
	token_data_t *floatval = createTokenDataFunction("floatval");
	htab_add(parser->globalSymTable, "floatval", floatval);

	token_data_t *intval = createTokenDataFunction("intval");
	htab_add(parser->globalSymTable, "intval", intval);

	token_data_t *strval = createTokenDataFunction("strval");
	htab_add(parser->globalSymTable, "strval", strval);

	token_data_t *reads = createTokenDataFunction("reads");
	htab_add(parser->globalSymTable, "reads", reads);

	token_data_t *readi = createTokenDataFunction("readi");
	htab_add(parser->globalSymTable, "readi", readi);

	token_data_t *readf = createTokenDataFunction("readf");
	htab_add(parser->globalSymTable, "readf", readf);

	token_data_t *write = createTokenDataFunction("write");
	htab_add(parser->globalSymTable, "write", write);

	token_data_t *strlen = createTokenDataFunction("strlen");
	htab_add(parser->globalSymTable, "strlen", strlen);

	token_data_t *substring = createTokenDataFunction("substring");
	htab_add(parser->globalSymTable, "substring", substring);

	token_data_t *ord = createTokenDataFunction("ord");
	htab_add(parser->globalSymTable, "ord", ord);

	token_data_t *chr = createTokenDataFunction("chr");
	htab_add(parser->globalSymTable, "chr", chr);

	parser->onParam = 0;
	parser->onParamType = 0;
	parser->onArg = 0;

	parser->ifCounter = 0;
	parser->whileCounter = 0;

	return parser;

freeDLList:
	DLLDispose(parser->codeGen);
	free(parser->codeGen);
freeNextToken:
	freeToken(parser->nextToken);
    free(parser->nextToken);
freeCurrentToken:
	freeToken(parser->currentToken);
    free(parser->currentToken);
freeHtab:
	htab_clear(parser->globalSymTable);
freeParser:
	free(parser);
returnNull:
	return NULL;
}

void destroyParser(Parser_t *parser)
{
	freeToken(parser->currentToken);
	freeToken(parser->nextToken);
	free(parser->currentToken);
	free(parser->nextToken);
	htab_free(parser->globalSymTable);
	empty_stack(parser->localSymStack);
	DLLDispose(parser->codeGen);

	free(parser);
	parser = NULL;
}

int getNextToken(Parser_t *parser)
{
	// it is just reassigning the pointer, currToken = nextToken, 
	// then putting the next incoming token into nextToken
	int ret = SUCCESS;

	token_t * temp = parser->currentToken;

	parser->currentToken = parser->nextToken;

	freeToken(temp);

	parser->nextToken = temp;

	if ((ret = scanToken(parser->nextToken)) != SUCCESS)
	{
		fprintf(stderr, "SCANNER ERROR (getNextToken): Exit with error code %d", ret);
		return ret;
	}

	return ret;
}

int checkTokenType(token_t* token, tokenType_t type)
{
	if (token->type == type)
	{
		return 0;
	}
	return 1;
}

int checkTokenKeyword(token_t* token, keyword_t keyword)
{
	if (token->type == TOK_KEYWORD)
	{
		if (token->attribute.kwVal == keyword)
		{
			return 0;
		}
	}
	return 1;
}
