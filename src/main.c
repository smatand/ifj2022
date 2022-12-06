/**
 * Project: Translator of language IFJ22
 * @file main.c
 * @author János László Vasík - xvasik05
 * @author Martin Maršalek - xmarsa15
 * @author Andrej Smatana - xsmata03
 *
 * @brief Main function of the compiler
*/

#include "parser.h"


int main()
{
    int ret = SUCCESS;

    // hardcoded check soo prologue is the first token read
	int c = getc(stdin);
	if(c != 60) // not beginning with prologue
    {
        fprintf(stderr, "LEXICAL ERROR: chars before prologue\n");
        exit(ERR_LEX_ANALYSIS);
    }
    ungetc(c,stdin); // return char if correct

    // initializing structures 
    Parser_t * parser;
    if((parser = initParser()) == NULL)
    {
        exit(ERR_INTERNAL);
    }

    // calling syntax analysis
    if((ret = rProgram(parser)) != SUCCESS)
    {
        fprintf(stderr, "PARSER ERROR (main): Exit with error code %d\n", ret);
    }

    destroyParser(parser);
    exit(ret);
}
