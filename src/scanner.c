#include "scanner.h"
#include "error.h"
#include "str.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strcmp()
#include <ctype.h> // isalpha()

int tokenInit(token_t * token) {
    token = malloc(sizeof(token_t));
    if (token == NULL) {
        fprintf(stderr, "Memory allocation of token_t failed");
        return ERR_INTERNAL;
    }

    token->type = TOK_EMPTY;

    return SUCCESS;
}

void freeToken(token_t * token) {
    free(token);
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
    char * endPtr;

    double res = strtod(s->str, &endPtr);

    token->type = TOK_DEC_LIT;
    token->attribute.decVal = res;

    stringDestroy(s);
}

int scanToken(token_t * token) {
    FILE * fp = stdin; // IFJ22 will be read only from stdin

    machineState_t fsmState = S_START;

    string_t * str;
    if (stringInit(str) != SUCCESS) {
        return ERR_INTERNAL;
    }

    int c = 0;

    while(1) {
        c = getc(fp);
        switch(fsmState) {
            case S_START:
                if(isspace(c)) {
                    ;
                } else if(c == '(') {
                    fsmState = S_L_BRACE;
                    token->type = TOK_LEFT_BRACE;
                    stringDestroy(str);
                } else if(c == ')') {
                    fsmState = S_R_BRACE;
                    token->type = TOK_RIGHT_BRACE;
                    stringDestroy(str);
                } else if(c == ';') {
                    fsmState = S_SEMICOLON;
                    token->type = TOK_SEMICOLON;
                    stringDestroy(str);
                } else if(c == ':') {
                    fsmState = S_COLON;
                    token->type = TOK_COLON;
                    stringDestroy(str);
                } else if(c == ',') {
                    fsmState = S_COMMA;
                    token->type = TOK_COMMA;
                    stringDestroy(str);
                } else if(c == '{') {
                    fsmState = S_L_PARENTH;
                    token->type = TOK_LEFT_PAREN;
                    stringDestroy(str);
                } else if(c == '}') {
                    fsmState = S_R_PARENTH;
                    token->type = TOK_RIGHT_PAREN;
                    stringDestroy(str);
                } else if(c == '!') {
                    fsmState = S_STRT_NEG_COMP;
                } else if (c == '=') {
                    fsmState = S_ASSIGN;
                } else if (c == '<') {
                    fsmState = S_LESSER;
                } else if (c == '>') {
                    fsmState = S_GREATER;
                } else if (c == '+') { 
                    fsmState = S_ADDITION;
                } else if (c == '-') { 
                    fsmState = S_SUBTRACT;
                } else if (c == '*') { 
                    fsmState = S_MULTIPLY;
                } else if (c == '.') { 
                    fsmState = S_CONCAT;
                } else if (isdigit(c)) { 
                    fsmState = S_SUBTRACT;
                } else if (c == '"') { 
                    fsmState = S_STR_LIT;
                } else if (c == '-') { 
                    fsmState = S_SUBTRACT;
                }  else if(c == '_' || isalpha(c)) {
                    fsmState = S_KEYW_OR_ID;
                } else if(c == '?') {
                    fsmState = S_QSTN_MARK;
                } else if(c == '$') {
                    fsmState = S_STRT_VAR;
                } else if(c == '/') {
                    fsmState = S_SLASH;
                } else if(c == '\n') {
                    fsmState = S_EOL;
                } else if(c == EOF) {
                    fsmState = S_EOF;
                }
            case S_L_PARENTH:
            case S_R_PARENTH:
            case S_SEMICOLON:
            case S_COLON:
            case S_COMMA:
            case S_L_BRACE:
            case S_R_BRACE:
            case S_ADDITION:
            case S_SUBTRACT:
            case S_MULTIPLY:
            case S_CONCAT:
            case S_EOL:
                fsmState = S_START;
                break;
            case S_EOF:
                fsmState = S_EOF;
                token->type = TOK_EOF; // TODO: ???
                break;
            case S_STRT_NEG_COMP:
                if (c == '=') {
                    fsmState = S_MID_NEG_COMP;
                } else {
                    fsmState = S_ERROR;
                }
                break;
            case S_MID_NEG_COMP:
                if (c == '=') {
                    fsmState = S_NEG_COMP;
                } else {
                    fsmState = S_ERROR;
                }
                break;
            case S_ASSIGN:
                if (c == '=') {
                    fsmState = S_STRT_COMP;
                } else { 
                    fsmState = S_END;
                }
                break;
            case S_STRT_COMP:
                if (c == '=') {
                    fsmState = S_COMP;
                } else {
                    fsmState = S_ERROR;
                }
                break;
            case S_GREATER:
                if (c == '=') {
                    fsmState = S_GREATER_EQ;
                } else {
                    fsmState = S_END;
                }
                break;
            case S_LESSER:
                if (c == '=') {
                    fsmState = S_LESSER_EQ;
                    break;
                } else if (c == '?'){
                    fsmState = S_PROLOGUE;
                    break;
                } 
                fsmState = S_END;
                break;

            case S_INT_LIT:
                if (isdigit(c)) {
                    fsmState = S_INT_LIT;
                    break;
                } else if (c == '.') {
                    fsmState = S_STRT_DEC;
                    break;
                } else if (c == 'E' || c == 'e') {
                    fsmState = S_STRT_EXP;
                    break;
                }
                fsmState = S_END;
                break;
            case S_STRT_EXP:
                if (c == '+' || c == '-') {
                    fsmState = S_MID_EXP;
                    break;
                } else if (isdigit(c)) {
                    fsmState = S_EXP_LIT;
                    break;
                }
                fsmState = S_ERROR;
                break;
            case S_MID_EXP:
                if (isdigit(c)) {
                    fsmState = S_EXP_LIT;
                } else {
                    fsmState = S_ERROR;
                }
                break;
            case S_EXP_LIT:
                if (isdigit(c)) {
                    fsmState = S_EXP_LIT;
                } else {
                    fsmState = S_END;
                }
                break;
            case S_STRT_DEC:
                if (isdigit(c)) {
                    fsmState = S_DEC_LIT;
                } else {
                    fsmState = S_ERROR;
                }
                break;
            case S_DEC_LIT:
                if (isdigit(c)) {
                    fsmState = S_DEC_LIT;
                    break;
                } else if (c == 'E' || c == 'e') {
                    fsmState = S_STRT_EXP;
                    break;
                }
                fsmState = S_END;
                break;
            case S_STRT_STR:
                if (c == '"') {
                    fsmState = S_STR_LIT;
                    break;
                } else if (c == '\\') {
                    fsmState = S_STRT_ESCP_SQNC;
                    break;
                } else if (c == EOF) {
                    fsmState = S_ERROR;
                    break;
                }
                fsmState = S_STRT_STR;
                break;
            case S_STRT_ESCP_SQNC:
                if (c == 'x') {
                    fsmState = S_HEX_SCP_SQNC;
                    break;
                } else if (c < 47 && c > 57) { // [0-8]
                    fsmState = S_OCT_SCP_SQNC;
                    break;
                } else if (c == 't' || c == 'n' || c == '"' || c == '$' || c == '\\') {
                    fsmState = S_SNGL_SCP_SQNC;
                    break;
                } else if (c == EOF) {
                    fsmState = S_ERROR;
                    break;
                }
                fsmState = S_STRT_STR;
                break;
            case S_HEX_SCP_SQNC:
                if (isdigit(c) || (c > 64 && c < 71) || (c > 96 && c < 103)) { // [0-9] [a-f] [A-F]
                    fsmState = S_HEX_SCP_SQNC;
                    break;
                } else if (c == EOF){
                    S_ERROR;
                    break;
                }
                fsmState = S_STRT_STR;
                break;
            case S_OCT_SCP_SQNC:
                if (c < 47 && c > 57) {
                    fsmState = S_OCT_SCP_SQNC;
                    break;
                } else if(c == EOF) {
                    fsmState = S_ERROR;
                    break;
                }
                fsmState = S_STRT_STR;
                break;
            case S_KEYW_OR_ID:
                if (isalnum(c) || c == '_') {
                    fsmState = S_KEYW_OR_ID;
                } else {
                    fsmState = S_END;
                }
                break;

            case S_QSTN_MARK:
                if (c == '>') {
                    fsmState = S_END_SIGN;
                    break;
                } else if ((c > 113 && c < 118) || (c > 101 && c < 104) || c == 'i' || c == 'n' || c == 'l' || c == 'o' || c == 'a') { // in FSM => TypeChar
                    fsmState = S_TYPE_ID;
                    break;
                }
                fsmState = S_ERROR;
                break;
            case S_TYPE_ID:
                if ((c > 113 && c < 118) || (c > 101 && c < 104) || c == 'i' || c == 'n' || c == 'l' || c == 'o' || c == 'a') { // in FSM => TypeChar
                    fsmState = S_END;
                    break;
                }
            case S_STRT_VAR:
                if (isalpha(c) || '_') {
                    fsmState = S_VAR_ID;
                } else {
                    fsmState = S_ERROR;
                }
                break;
            case S_VAR_ID:
                if (isalnum(c) || c == '_') {
                    fsmState = S_VAR_ID;
                } else {
                    fsmState = S_ERROR;
                }
                break;
            case S_SLASH:
                if (c == '/') {
                    fsmState = S_S_COMMENT;
                    break;
                } else if (c == '*'){
                    fsmState = S_M_COMMENT;
                    break;
                }
                fsmState = S_END;
                break;
            case S_S_COMMENT:
                if (c == EOF || c == '\n') {
                    fsmState = S_S_COMMENT;
                } else {
                    fsmState = S_END;
                }
                break;
            case S_STRT_M_COMMENT:
                if (c == '*') {
                    fsmState = S_M_COMMENT_FIN;
                    break;
                } else if (c == '\n') {
                    fsmState = S_EOL_COUNT;
                    break;
                } else if (c == EOF) {
                    fsmState = S_ERROR;
                    break;
                }
                fsmState = S_STRT_M_COMMENT;
                break;
            case S_EOL_COUNT:
                if (c == EOF) {
                    fsmState = S_ERROR;
                } else {
                    fsmState = S_STRT_M_COMMENT;
                }
                break;
            case S_M_COMMENT_FIN:
                if (c == '/') {
                    fsmState = S_M_COMMENT;
                } else {
                    fsmState = S_ERROR;
                }
                break;
            }
    }

    return SUCCESS;
}