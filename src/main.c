/**
 * @file main.c
 * @author János László Vasík
 *
 * @brief Main function of the compiler
*/

#include "parser.h"


int main()
{
    int ret = SUCCESS;
    // initializing structures 
    Parser_t * parser;
    if(parser = initParser() == NULL)
    {
        exit(ERR_INTERNAL);
    }

    // calling syntax analysis, first pass
    if((ret = parseSource(parser)) != SUCCESS)
    {
        fprintf(stderr, "PARSER ERROR (main): Exit with error code %d", ret);
    }

    parser->firstPass = false;

    // calling syntax analysis, second pass
    if((ret = parseSource(parser)) != SUCCESS)
    {
        fprintf(stderr, "PARSER ERROR (main): Exit with error code %d", ret);
    }

    exit(ret);
}
