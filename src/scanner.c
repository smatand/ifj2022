#include "scanner.h"
#include "error.h"
#include "str.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strcmp()

int tokenInit(token_t * token) {
    token = calloc(1, sizeof(token_t));
    if (token == NULL) {
        fprintf(stderr, "Memory allocation of token_t failed");
        return ERR_INTERNAL;
    }

    return SUCCESS;
}

void freeToken(token_t * token) {
    free(token);
}

int scanToken(token_t * token) {
    token->type = TOK_EMPTY;
    /*
    TODO
    */
    return SUCCESS;
}

int checkKeyword(token_t * token, string_t * s) {
    if (!strcmp(s->str, "if")) {
        token->attribute.kwVal = KW_IF;
    } else if (!strcmp(s->str, "else")) {
        token->attribute.kwVal = KW_ELSE;
    } else if (!strcmp(s->str, "int")) {
        token->attribute.kwVal = KW_INT;
    } else if (!strcmp(s->str, "float")) {
        token->attribute.kwVal = KW_FLOAT;
    } else if (!strcmp(s->str, "function")) {
        token->attribute.kwVal = KW_FUNCTION;
    } else if (!strcmp(s->str, "null")) {
        token->attribute.kwVal = KW_NULL;
    } else if (!strcmp(s->str, "return")) {
        token->attribute.kwVal = KW_RETURN;
    } else if (!strcmp(s->str, "string")) {
        token->attribute.kwVal = KW_STRING;
    } else if (!strcmp(s->str, "void")) {
        token->attribute.kwVal = KW_VOID;
    } else if (!strcmp(s->str, "while")) {
        token->attribute.kwVal = KW_WHILE;
    } else if (!strcmp(s->str, "true")) {
        token->attribute.kwVal = KW_TRUE;
    } else if (!strcmp(s->str, "false")) {
        token->attribute.kwVal = KW_FALSE;
    } else if (!strcmp(s->str, "php")) {
        token->attribute.kwVal = KW_PHP;
    } else {
        return 0; // no keyword found
    }

    stringDestroy(s);

    return 1;
}

void convertStringToInt(string_t * s, token_t * token) {
    // todo: make it converting hexadecimal too
    char * endPtr;

    int res = strtol(s->str, &endPtr, 10);

    token->type = TOK_INT_LIT;
    token->attribute.intVal = res;

    stringDestroy(s);
}

void convertStringToDouble(string_t * s, token_t * token) {
    char * endPtr

    double res = strtod(s->str, &endPtr, 10);

    token->type = TOK_DEC_LIT;
    token->attribute.decVal = res;

    stringDestroy(s);
}