/**
 * @file parser.c
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
		return NULL;
	}

	if ((parser->globalSymTable = htab_init(INITIAL_BUCKET_COUNT)) == NULL)
	{
		free(parser);
		return NULL;
	}

	if ((parser->currentToken = tokenInit()) == NULL) // allocate memory for the first token
	{
		htab_clear(parser->globalSymTable);
		free(parser);
		return NULL;
	}

	if ((parser->nextToken = tokenInit() == NULL)) // allocate memory for the second token
	{
		htab_clear(parser->globalSymTable);
		freeToken(parser->currentToken);
		free(parser);
		return NULL;
	}

	if (scanToken(parser->currentToken) != SUCCESS) // scan first token
	{
		htab_clear(parser->globalSymTable);
		freeToken(parser->currentToken);
		freeToken(parser->nextToken);
		free(parser);
		return NULL;
	}

	if (scanToken(parser->nextToken) != SUCCESS) // scan second token
	{
		htab_clear(parser->globalSymTable);
		freeToken(parser->currentToken);
		freeToken(parser->nextToken);
		free(parser);
		return NULL;
	}

	init_stack(parser->localSymStack);
	push_empty(parser->localSymStack);

	parser->latestFuncDeclared = NULL;
	parser->latestFuncCalled = NULL;
	parser->latestVar = NULL;

	parser->firstPass = true;
	parser->currentArgument = 0;

	return parser;
}

void destroyParser(Parser_t *parser)
{
	freeToken(parser->currentToken);
	freeToken(parser->nextToken);
	htab_free(parser->globalSymTable);
	destroy_stack(parser->localSymStack);

	free(parser);
	parser = NULL;
}

int parseSource(Parser_t *parser)
{
	int ret = SUCCESS;
	ret = rProgram(parser);

	return ret;
}

int getNextToken(Parser_t *parser)
{
	int ret = SUCCESS;
	parser->currentToken = parser->nextToken;
	if ((ret = scanToken(parser->nextToken)) != SUCCESS)
	{
		fprintf(stderr, "SCANNER ERROR (getNextToken): Exit with error code %d", ret);
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

void setLatestFuncID(Parser_t *parser, htab_item_t *ID)
{
	parser->latestFuncDeclared = ID;
}
