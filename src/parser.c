/**
 * @file parser.c
 *
 * @brief Implementation of parser module for IFJ22
 */

#include "parser.h"

Parser_t *initParser()
{
	Parser_t *parser = malloc(sizeof(Parser_t));

	if (parser == NULL)
	{
		fprintf(stderr, "PARSER ERROR (initParser): Parser struct allocation failure");
		return NULL;
	}

	if ((parser->symTable = htab_init(INITIAL_BUCKET_COUNT)) == NULL)
	{
		fprintf(stderr, "PARSER ERROR (initParser): Parser symtable allocation failure");
		free(parser);
		parser = NULL;
	}

	if (tokenInit(parser->currentToken) != SUCCESS) // scan and store first token
	{
		fprintf(stderr, "PARSER ERROR (initParser): Parser current token initialization failure");
		free(parser);
		parser = NULL;
	}

	if (tokenInit(parser->nextToken) != SUCCESS) // scan and store first token
	{
		fprintf(stderr, "PARSER ERROR (initParser): Parser next token initialization failure");
		free(parser);
		parser = NULL;
		freeToken(parser->currentToken);
	}

	if (scanToken(parser->currentToken) != SUCCESS) // scan and store first token
	{
		fprintf(stderr, "PARSER ERROR (initParser): Parser current token scan failure");
		free(parser);
		parser = NULL;
		freeToken(parser->currentToken);
		freeToken(parser->nextToken);
	}

	if (scanToken(parser->nextToken) != SUCCESS) // scan and store second token
	{
		fprintf(stderr, "PARSER ERROR (initParser): Parser next token scan failure");
		free(parser);
		parser = NULL;
		freeToken(parser->currentToken);
		freeToken(parser->nextToken);
	}

	return parser;
}

int destroyParser(Parser_t *parser)
{
	freeToken(parser->currentToken);
	freeToken(parser->nextToken);
	htab_free(parser->symTable);

	free(parser);
	parser = NULL;
}

int parseSource()
{
	int error;
	error = program();

	return 0;
}

int getNextToken(Parser_t *parser)
{
	parser->currentToken = parser->nextToken;
	if (scanToken(parser->currentToken) != SUCCESS)
	{
		fprintf(stderr, "PARSER ERROR (getNextToken): Scan next token failure");
		return ERR_INTERNAL;
	}

	return SUCCESS;
}