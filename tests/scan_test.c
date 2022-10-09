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
                printf("TOK_LEFT_PAREN\n");
                break;
            case TOK_RIGHT_PAREN:
                printf("TOK_RIGHT_PAREN\n");
                break;
            case TOK_PROLOGUE:
                printf("TOK_PROLOGUE\n");
                break;
            case TOK_END_PROLOGUE:
                printf("TOK_END_PROLOGUE\n");
                break;
            case TOK_EMPTY: // comments
                printf("\n");
                break;
            case TOK_EOF:
                printf("TOK_EOF\n");
                break;
            case TOK_KEYWORD:
                switch (token->attribute.kwVal) {
                    case KW_INT:
                        printf("KW_INT\n");
                        break;
                    case KW_FLOAT:
                        printf("KW_FLOAT\n");
                        break;
                    case KW_STRING:
                        printf("KW_STRING\n");
                        break;
                    case KW_NULL:
                        printf("KW_NULL\n");
                        break;
                }
                break;
            case TOK_INT_LIT:
                printf("TOK_INT_LIT: %d\n", token->attribute.intVal);
                break;
            case TOK_DEC_LIT:
                printf("TOK_DEC_LIT: %f\n", token->attribute.decVal);
                break;
            default:
                printf("unknown");
                break;
        }

        scanToken(token);
    }

    freeToken(token);
    return 0;
}
