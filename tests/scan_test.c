#include <stdio.h>
#include <stdlib.h>
#include "../src/scanner.h"
#include "../src/error.h"

int main() {
    token_t * token = calloc(1, sizeof(token_t));
    if (tokenInit(token) != SUCCESS) {
        return ERR_INTERNAL;
    }

    scanToken(token); // first scan

    while (token->type != TOK_EOF) {
        switch (token->type) {
            case TOK_LEFT_BRACE:
                printf("TOK_LEFT_BRACE\n");
                break;
            case TOK_RIGHT_BRACE:
                printf("TOK_RIGHT_BRACE\n");
                break;
            case TOK_SEMICOLON:
                printf("TOK_SEMICOLON\n");
                break;
            case TOK_COLON:
                printf("TOK_COLON\n");
                break;
            case TOK_COMMA:
                printf("TOK_COMMA\n");
                break;
            case TOK_LEFT_PAREN:
                printf("TOK_LEFT_PAREN");
                break;
            case TOK_RIGHT_PAREN:
                printf("TOK_RIGHT_PAREN");
                break;
            case TOK_EOF:
                printf("TOK_EOF");
                break;
        }

        scanToken(token);
    }

    return 0;
}
