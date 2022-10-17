#include <stdio.h>
#include <stdlib.h>
#include "../src/scanner.h"
#include "../src/error.h"

int main() {
    token_t * token = calloc(1, sizeof(token_t));
    if (tokenInit(token) != SUCCESS) {
        return ERR_INTERNAL;
    }

    if (scanToken(token) == ERR_LEX_ANALYSIS) {
        printf("RETURNED ERR_LEX_ANALYSIS\n");
        token->type = TOK_EMPTY;
    }

    while (token->type != TOK_EOF) {
        switch (token->type) {
            case TOK_EMPTY: // comments
                printf("\n");
                break;

            case TOK_ERROR: 
                printf("TOK_ERROR\n");
                break;

            case TOK_LEFT_PAREN:
                printf("TOK_LEFT_PAREN\n");
                break;
            case TOK_RIGHT_PAREN:
                printf("TOK_RIGHT_PAREN\n");
                break;
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

            case TOK_DOT:
                printf("TOK_DOT\n");
                break;
            case TOK_MINUS:
                printf("TOK_MINUS\n");
                break;
            case TOK_PLUS:
                printf("TOK_PLUS\n");
                break;
            case TOK_SLASH:
                printf("TOK_SLASH\n");
                break;
            case TOK_STAR:
                printf("TOK_STAR\n");
                break;
            case TOK_NEG_COMPARISON:
                printf("TOK_NEG_COMPARISON\n");
                break;
            case TOK_ASSIGN:
                printf("TOK_ASSIGN\n");
                break;
            case TOK_COMPARISON:
                printf("TOK_COMPARISON\n");
                break;
            case TOK_GREATER:
                printf("TOK_GREATER\n");
                break;
            case TOK_GREATER_EQUAL:
                printf("TOK_GREATER_EQUAL\n");
                break;
            case TOK_LESS:
                printf("TOK_LESS\n");
                break;
            case TOK_LESS_EQUAL:
                printf("TOK_LESS_EQUAL\n");
                break;

            case TOK_IDENTIFIER:
                printf("TOK_IDENTIFIER: %s\n", token->attribute.strVal);
                break;
            case TOK_TYPE_ID:
                switch (token->attribute.kwVal) {
                    case KW_INT:
                        printf("TOK_TYPE_ID: KW_INT\n");
                        break;
                    case KW_FLOAT:
                        printf("TOK_TYPE_ID: KW_FLOAT\n");
                        break;
                    case KW_STRING:
                        printf("TOK_TYPE_ID: W_STRING\n");
                        break;
                }
                break;
            
            case TOK_STRING_LIT:
                printf("TOK_STRING_LIT: %s", token->attribute.strVal);
                break;
            case TOK_INT_LIT:
                printf("TOK_INT_LIT: %d\n", token->attribute.intVal);
                break;
            case TOK_DEC_LIT:
                printf("TOK_DEC_LIT: %f\n", token->attribute.decVal);
                break;

            case TOK_KEYWORD:
                switch (token->attribute.kwVal) {
                    case KW_IF:
                        printf("KW_IF\n");
                        break;
                    case KW_ELSE:
                        printf("KW_ELSE\n");
                        break;
                    case KW_INT:
                        printf("KW_INT\n");
                        break;
                    case KW_FLOAT:
                        printf("KW_FLOAT\n");
                        break;
                    case KW_FUNCTION:
                        printf("KW_FUNCTION\n");
                        break;
                    case KW_NULL:
                        printf("KW_NULL\n");
                        break;
                    case KW_RETURN:
                        printf("KW_RETURN\n");
                        break;
                    case KW_STRING:
                        printf("KW_STRING\n");
                        break;
                    case KW_VOID:
                        printf("KW_VOID\n");
                        break;
                    case KW_WHILE:
                        printf("KW_WHILE\n");
                        break;
                    case KW_TRUE:
                        printf("KW_TRUE\n");
                        break;
                    case KW_FALSE:
                        printf("KW_FALSE\n");
                        break;
                }
                break;

            case TOK_PROLOGUE:
                printf("TOK_PROLOGUE\n");
                break;
            case TOK_END_PROLOGUE:
                printf("TOK_END_PROLOGUE\n");
                break;

            case TOK_EOF:
                printf("TOK_EOF\n");
                break;
            default:
                printf("unknown");
                break;
        }

        // we want to test the edge cases without terminating the program
        while (scanToken(token) == ERR_LEX_ANALYSIS) {
            printf("RETURNED ERR_LEX_ANALYSIS\n");
            token->type = TOK_EMPTY;
        }
    }

    freeToken(token);
    return 0;
}
