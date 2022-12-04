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

	if ((scanToken(parser->currentToken)) != SUCCESS) // scan first token
	{
		goto freeNextToken;
	}

	if ((scanToken(parser->nextToken)) != SUCCESS) // scan second token
	{
		goto freeNextToken;
	}

	if ((init_stack(&(parser->localSymStack))) != SUCCESS)
	{
		goto freeNextToken;
	}

	push_empty(parser->localSymStack); // TODO no effect, no body in function sym_table_stack.c

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

	parser->latestFuncDeclared = NULL;
	parser->latestFuncCalled = NULL;
	parser->latestVar = NULL;

	return parser;

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
	return NULL; // ERR_INTERNAL
}

void destroyParser(Parser_t *parser)
{
	freeToken(parser->currentToken);
	freeToken(parser->nextToken);
	free(parser->currentToken);
	free(parser->nextToken);
	htab_free(parser->globalSymTable);
	empty_stack(parser->localSymStack);

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

	// it is just reassigning the pointer, now currToken = nextToken
	//parser->currentToken = parser->nextToken; 

	token_t * temp = parser->currentToken;

	parser->currentToken = parser->nextToken;

	freeToken(temp); // just the string freeing

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

void setLatestFuncID(Parser_t *parser, htab_item_t *ID)
{
	parser->latestFuncDeclared = &ID->pair;
}

