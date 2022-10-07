#include <stdlib.h>
#include "../src/scanner.h"

int main() {
    FILE * fp = stdin;
    token_t * token = calloc(1, sizeof(token_t));

    get_token(fp, token); // load first token 

    while (token->type != T_EOF) {
        switch (token->type) {
            //////////////////PROLOGUE//////////////////////////
            case TOK_PROLOGUE:
                printf("TOK_PROLOGUE: %s)", token->attribute); /* <? */
                break;
            case TOK_KEY_ID:
                printf("TOK_KEY_ID: %s)", token->attribute); /* php */ // (or other keywords ?? todo)
                break;
            case TOK_END_SIGN:
                printf("TOK_END_SIGN: %s)", token->attribute); /* ?> */
                break;
            ///////////////////NUMBERS//////////////////////////
            case TOK_INT_LIT:
                printf("TOK_INT_LIT: %d)", token->attribute); /* 123 */
                break;
            case TOK_DEC_LIT:
                printf("TOK_DEC_LIT: %f)", token->attribute); /* 123.456 */
                break;
            ///////////////////KEYWORDS//////////////////////////
            case TOK_WHILE:
                printf("TOK_WHILE: %s)", token->attribute); /* while */
                break;
            case TOK_IF:
                printf("TOK_IF: %s)", token->attribute); /* if */
                break;
            case TOK_VOID:
                printf("TOK_VOID: %s)", token->attribute); /* void */
                break;
            case TOK_INT:
                printf("TOK_INT: %s)", token->attribute); /* int */
                break;
            case TOK_FALSE:
                printf("TOK_FALSE: %s)", token->attribute); /* false */
                break;
            case TOK_ELSE:
                printf("TOK_ELSE: %s)", token->attribute); /* else */
                break;
            default:
                break;
            }

        // load next token
        if (get_token(fp, token) != 0) {
            printf("ERROR");
        }
    }

    return 0;
}
