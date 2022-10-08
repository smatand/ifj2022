#include "scanner.h"
#include "error.h"

#include <stdlib.h>
#include <stdio.h>

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