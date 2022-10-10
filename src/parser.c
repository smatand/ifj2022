/**
 * @file parser.c
 *
 * @brief Implementation of parser module for IFJ22
 */

#include "parser.h"

Parser_t* initParser(FILE *stream)
{
	Parser_t* parser = malloc(sizeof(Parser_t));
	if (parser == NULL)
	{
		fprintf(stderr, "Memory allocation failed: Struct --- parser.");
		return NULL;
	}

	parser->scanner = scannerInit(stream);
	if (parser->scanner == NULL)
	{
		free(parser);
		return NULL;
	}

	return parser;
}

int destroyParser(Parser_t* parser)
{
	// TODO: destroyScanner(parser->scanner);
	// free(parser);
	// parser = NULL;
}

int parseSource()
{
	int error;
	error = program();

	return 0;
}