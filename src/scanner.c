#include "scanner.h"
#include "error.h"
#include "str.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strcmp()
#include <ctype.h> // isalpha()

#define MAX_KEYWORD_CHARS 9 // 8 + 1 for /0 (function + '\0')

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
    } else {
        return 0; // no keyword found, it is ID
    }

    token->type = TOK_KEYWORD;
    return 1; // caller must take care of stringDestroy()!
}

void convertStringToInt(string_t * s, token_t * token) {
    // todo: make it converting hexadecimal too
    char * endPtr;

    int res = strtol(s->str, &endPtr, 10);

    token->type = TOK_INT_LIT;
    token->attribute.intVal = res;
}

void convertStringToDouble(string_t * s, token_t * token) {
    char * endPtr;

    double res = strtod(s->str, &endPtr);

    token->type = TOK_DEC_LIT;
    token->attribute.decVal = res;
}

int convertHexToInt(char * s){
    char * endPtr;

    int res = strtol(s, &endPtr, 16);

    return res;
}

int checkForPrologue(FILE * fp) {
    // just simply get first 3 chars and compare them with "php"
    char prologue[4] = "php";
    
    char loadedChars[4];
    int c = 0;

    for (int i=0; i<3; i++) {
        c = getc(fp);

        if (c != EOF) {
            loadedChars[i] = c;
        } else {
            return ERR_LEX_ANALYSIS;
        }
    }
    loadedChars[3] = '\0';

    return strcmp(loadedChars, prologue);
}

int fillStrWithKeyword(string_t * s, FILE * fp) {
    char buff[MAX_KEYWORD_CHARS];
    int c = getc(fp);
    int i = 0;

    while (c != EOF && !isspace(c) && c != '=') {
        buff[i++] = c;
        c = getc(fp);

        if (i == MAX_KEYWORD_CHARS) {
            return ERR_LEX_ANALYSIS;
        }
    }
    
    if (c == EOF) {
        return ERR_LEX_ANALYSIS;
    }

    buff[i] = '\0';

    memcpy(s->str, buff, i); // s->str should contain DEFAULT_LEN (16) space
    s->realLen = i;

    return SUCCESS;
}

int fillStr(string_t * s, token_t * token, FILE * fp) {
    char * buff = calloc(1, MAX_KEYWORD_CHARS);
    if (buff == NULL) {
        fprintf(stderr, "ERROR %d ALLOCATION FAILED", __LINE__);
        return ERR_INTERNAL;
    }

    int c = getc(fp);
    int i = 0;

    while (c != EOF && !isspace(c) && c != '=' && (isalnum(c) || c == '_')) {
        buff[i++] = c;
        c = getc(fp);

        if (i == MAX_KEYWORD_CHARS) {
            buff = realloc(buff, i + MAX_KEYWORD_CHARS);

            if (buff == NULL) {
                //free(buff);
                fprintf(stderr, "ERROR %d ALLOCATION FAILED", __LINE__);
                return ERR_INTERNAL;
            }
        }
    }

    if (c == EOF) {
        free(buff);
        fprintf(stderr, "%d ERROR EOF REACHED\n", __LINE__);
        return ERR_LEX_ANALYSIS;
    }

    buff[i] = '\0';

    if (stringResize(s, i + MAX_KEYWORD_CHARS) != SUCCESS) {
        free(buff);
        fprintf(stderr, "ERROR %d ALLOCATION FAILED", __LINE__);
        return ERR_INTERNAL;
    }

    memcpy(s->str, buff, i);
    s->realLen = i;

    token->type = TOK_IDENTIFIER;
    token->attribute.strVal = calloc(1, s->realLen);

    if (token->attribute.strVal == NULL) {
        free(buff);
        fprintf(stderr, "ERROR %d ALLOCATION FAILED", __LINE__);
        return ERR_INTERNAL; // caller must take care of stringDestroy()!
    }

    memcpy(token->attribute.strVal, s->str, s->realLen);

    // caller must take care of stringDestroy()!
    ungetc(c, fp);
    free(buff);
    return SUCCESS;
}

int scanToken(token_t * token) {
    FILE * fp = stdin; // IFJ22 will be read only from stdin

    machineState_t fsmState = S_START;

    int ret = 0;
    string_t * str = stringInit(&ret);
    if (ret != SUCCESS) {
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
                    token->type = TOK_LEFT_BRACE;

                    stringDestroy(str);
                    return SUCCESS;
                } else if(c == ')') {
                    token->type = TOK_RIGHT_BRACE;

                    stringDestroy(str);
                    return SUCCESS;
                } else if(c == ';') {
                    token->type = TOK_SEMICOLON;

                    stringDestroy(str);
                    return SUCCESS;
                } else if(c == ':') {
                    token->type = TOK_COLON;

                    stringDestroy(str);
                    return SUCCESS;
                } else if(c == ',') {
                    token->type = TOK_COMMA;

                    stringDestroy(str);
                    return SUCCESS;
                } else if(c == '{') {
                    token->type = TOK_LEFT_PAREN;

                    stringDestroy(str);
                    return SUCCESS;
                } else if(c == '}') {
                    token->type = TOK_RIGHT_PAREN;

                    stringDestroy(str);
                    return SUCCESS;
                } else if(c == '!') {
                    fsmState = S_STRT_NEG_COMP;
                } else if (c == '=') {
                    fsmState = S_ASSIGN;
                } else if (c == '<') {
                    // if there is a char other than '=' or '?' after, it is just the < operator
                    int c2 = lookAheadByOneChar(fp);
                    if (c2 != '=' && c2 != '?') {
                        token->type = TOK_LESS;

                        stringDestroy(str);
                        return SUCCESS;
                    }

                    fsmState = S_LESSER;
                } else if (c == '>') {
                    if (lookAheadByOneChar(fp) != '=') {
                        token->type = TOK_GREATER; // S_GREATER

                        stringDestroy(str);
                        return SUCCESS;
                    } else {
                        token->type = TOK_GREATER_EQUAL; // S_GREATER_EQ

                        stringDestroy(str);
                        return SUCCESS;
                    }
                } else if (c == '+') { 
                    token->type = TOK_PLUS;

                    stringDestroy(str);
                    return SUCCESS;
                } else if (c == '-') { 
                    token->type = TOK_MINUS;

                    stringDestroy(str);
                    return SUCCESS;
                } else if (c == '*') { 
                    token->type = TOK_STAR;

                    stringDestroy(str);
                    return SUCCESS;
                } else if (c == '.') { 
                    token->type = TOK_DOT;

                    stringDestroy(str);
                    return SUCCESS;
                } else if (c == '"') { 
                    fsmState = S_STRT_STR;
                }  else if(c == '_' || isalpha(c)) {
                    fsmState = S_KEYW_OR_ID;
                    ungetc(c, fp);
                } else if(c == '?') {
                    fsmState = S_QSTN_MARK;
                } else if(c == '$') {
                    fsmState = S_STRT_VAR;
                } else if(c == '/') {
                    fsmState = S_SLASH;
                    token->type = TOK_SLASH;
                } else if(c == '\n') {
                    fsmState = S_EOL;
                } else if (isdigit(c)) {
                    fsmState = S_INT_LIT;
                    if (strPushBack(str, c) != SUCCESS) {
                        return ERR_INTERNAL;
                    }
                } else if(c == EOF) {
                    token->type = TOK_EOF;
                    stringDestroy(str);
                    return SUCCESS;
                }
                break;
            case S_EOL:
                fsmState = S_START;
                break;
            case S_STRT_NEG_COMP:
                if (c == '=') {
                    fsmState = S_MID_NEG_COMP;
                } else {
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS; // S_ERROR
                }
                break;
            case S_MID_NEG_COMP:
                if (c == '=') {
                    token->type = TOK_NEG_COMPARISON; // S_NEG_COMP

                    stringDestroy(str);
                    return SUCCESS;
                } else {
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS; // S_ERROR
                }
            case S_ASSIGN:
                if (c == '=') {
                    fsmState = S_STRT_COMP; 
                } else { 
                    token->type = TOK_ASSIGN;

                    stringDestroy(str);
                    return SUCCESS;
                }
                break;
            case S_STRT_COMP:
                if (c == '=') {
                    token->type = TOK_COMPARISON;

                    stringDestroy(str);
                    return SUCCESS; // S_COMPARISON (nonexistent)
                } else {
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS; // S_ERROR
                }
            case S_LESSER:
                if (c == '=') {
                    token->type = TOK_LESS_EQUAL;

                    stringDestroy(str);
                    return SUCCESS; // S_LESSER_EQ
                } else if (c == '?'){
                    if(!checkForPrologue(fp)) {
                        token->type = TOK_PROLOGUE; // TOK_PROLOGUE is the whole expression "<?php" TODO should it be taken as syntax error (2) ?

                        stringDestroy(str);
                        return SUCCESS;
                    } else {
                        token->type = TOK_ERROR;

                        stringDestroy(str);
                        return ERR_LEX_ANALYSIS; // prologue was not loaded
                    }
                } 
                break;
            case S_INT_LIT:
                if (isdigit(c)) {
                    if (strPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                } else if (c == '.') {
                    if (strPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_STRT_DEC;
                } else if (c == 'E' || c == 'e') {
                    if (strPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_STRT_EXP;
                } else {
                    convertStringToInt(str, token);

                    ungetc(c, fp); // return last read char to stream if it's not 'E', 'e', '.', or a number

                    stringDestroy(str);
                    return SUCCESS;
                }
                break;
            case S_STRT_EXP:
                if (c == '+' || c == '-') {
                    if (strPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_MID_EXP;
                    break;
                } else if (isdigit(c)) {
                    if (strPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_EXP_LIT;
                    break;
                } else {
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS; // S_ERROR, missing number or +- 
                }
            case S_MID_EXP:
                if (isdigit(c)) {
                    if (strPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }

                    fsmState = S_EXP_LIT;
                } else {
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS; // S_ERROR, missing number
                }
            case S_EXP_LIT:
                if (isdigit(c)) {
                    if (strPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                } else {
                    convertStringToDouble(str, token); // uses strdol() function, which works with exponents

                    stringDestroy(str);
                    return SUCCESS;
                }
                break;
            case S_STRT_DEC:
                if (isdigit(c)) {
                    if (strPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_DEC_LIT;
                    break;
                } else {
                    return ERR_LEX_ANALYSIS;
                }
            case S_DEC_LIT:
                if (isdigit(c)) {
                    if (strPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    break;
                } else if (c == 'E' || c == 'e') {
                    fsmState = S_STRT_EXP;
                    break;
                } else {
                    convertStringToDouble(str, token);

                    stringDestroy(str);
                    return SUCCESS;
                }
            case S_STRT_STR:
                if (c == '"') {
                    token->type = TOK_STRING_LIT;
                    strcpy(token->attribute.strVal, str->str);
                    stringDestroy(str);
                    return SUCCESS;
                } else if (c == '\\') {
                    fsmState = S_STRT_ESCP_SQNC;
                } else if (c == EOF) {
                    token->type = TOK_ERROR;
                    strcpy(token->attribute.strVal, str->str);
                    return ERR_LEX_ANALYSIS;
                } else { // else it is part of string literal
                    if (strPushBack(str, c) != SUCCESS) {
                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_STRT_STR;
                }
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
                if (strPushBack(str, c) != SUCCESS) { // if no valid escapes were read, treat it as part of the string
                    stringDestroy(str);
                    return ERR_INTERNAL;
                }
                fsmState = S_STRT_STR;
                break;
            case S_HEX_SCP_SQNC:
                char escpStr[5] = {'0', 'x', '\0', '\0', '\0'}; // convertible format of hexa number
                if (isdigit(c) || (c > 64 && c < 71) || (c > 96 && c < 103)) { // [0-9] [a-f] [A-F] first hexa char
                    escpStr[2] = c;
                    int temp = lookAheadByOneChar(fp);
                    if (isdigit(temp) || (temp > 64 && temp < 71) || (temp > 96 && temp < 103)) { // second hexa char
                        escpStr[3] = c;
                        temp = convertHexToInt(escpStr);
                        if (temp > 32 || temp < 127) { // if convertible to a printable and allowed char, will do so
                            if (strPushBack(str, temp) != SUCCESS) {
                                stringDestroy(str);
                                return ERR_INTERNAL;
                            }
                            fsmState = S_STRT_STR;
                            break;
                        } else { // else append the escape sequence to the end of the string literal
                            escpStr[0] = '\\'; // putting back the backslash
                            for (size_t i = 0; i < 5; i++){
                                if (strPushBack(str, escpStr[i]) != SUCCESS) {
                                    stringDestroy(str);
                                    return ERR_INTERNAL;
                                }
                            }
                            fsmState = S_STRT_STR;
                            break;
                        }
                    } else if (c == EOF){ // error caused by EOF
                        token->type = TOK_ERROR;
                        for (size_t i = 0; i < 4; i++){
                            if (strPushBack(str, escpStr[i]) != SUCCESS) {
                                stringDestroy(str);
                                return ERR_INTERNAL;
                            }
                        }
                        strcpy(token->attribute.strVal, str->str);
                        return ERR_LEX_ANALYSIS;
                    } else { // incorrect hexa escp. seq. -> part of string
                        for (size_t i = 0; i < 4; i++){
                            if (strPushBack(str, escpStr[i]) != SUCCESS) {
                                stringDestroy(str);
                                return ERR_INTERNAL;
                            }
                        }
                        fsmState = S_STRT_STR;
                        break;
                    }
                } else if (c == EOF){
                    token->type = TOK_ERROR;
                    for (size_t i = 0; i < 3; i++){
                        if (strPushBack(str, escpStr[i]) != SUCCESS) {
                            stringDestroy(str);
                            return ERR_INTERNAL;
                        }
                    }
                    strcpy(token->attribute.strVal, str->str);
                    return ERR_LEX_ANALYSIS;
                } else {
                    escpStr[0] = '\\'; // putting back the backslash
                    for (size_t i = 0; i < 3; i++){ // pushing back "\x[invalid hexa char]"
                        if (strPushBack(str, escpStr[i]) != SUCCESS) {
                            stringDestroy(str);
                            return ERR_INTERNAL;
                        }
                    }
                    fsmState = S_STRT_STR;
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
                ungetc(c, fp); // again to manipulate with c correctly
                fillStr(str, token, fp);

                checkKeyword(token, str); // changes token->type

                stringDestroy(str);

                return SUCCESS;
            case S_QSTN_MARK:
                if (c == '>') {
                    token->type = TOK_END_PROLOGUE;

                    stringDestroy(str);
                    return SUCCESS;
                } else { 
                    ungetc(c, fp);

                    fsmState = S_TYPE_ID;
                    break;
                }
                stringDestroy(str);
                return ERR_LEX_ANALYSIS;
            case S_TYPE_ID:
                    // need to check for 'string', 'int' or 'float'
                    ungetc(c, fp);
                    if (fillStr(str, token, fp) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_LEX_ANALYSIS;
                    }

                    if (checkKeyword(token, str) == 0) {

                        stringDestroy(str); // it doesn't match any keyword, throw err_lex
                        return ERR_LEX_ANALYSIS;
                    } else if ( token->attribute.kwVal == KW_STRING ||  token->attribute.kwVal == KW_INT 
                            ||  token->attribute.kwVal == KW_FLOAT) {
                        
                        token->type = TOK_TYPE_ID;

                        stringDestroy(str);
                        return SUCCESS; // token->type is set, attribute too
                    } else { // other keywords are not allowed as TYPE
                        token->type = TOK_EMPTY;

                        stringDestroy(str);
                        return ERR_LEX_ANALYSIS;
                    }
                    break;
            case S_STRT_VAR:
                if (isalpha(c) || '_') {
                    ungetc(c, fp);

                    int ret = fillStr(str, token, fp);

                    stringDestroy(str);
                    return ret; // ERR_INTERNAL or SUCCESS
                } else {
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS;
                }
                break;
            case S_SLASH:
                if (c == '/') {
                    fsmState = S_S_COMMENT;
                    break;
                } else if (c == '*'){
                    fsmState = S_M_COMMENT;
                    break;
                } else {
                    token->type = TOK_SLASH;

                    ungetc(c, fp);
                    stringDestroy(str);
                    return SUCCESS;
                }
            case S_S_COMMENT:
                if (c == EOF || c == '\n') {
                    token->type = TOK_EMPTY;

                    stringDestroy(str);
                    return SUCCESS;
                }
                break;
            // multiline comments
            case S_M_COMMENT: 
                if (c == '*') {
                    if (lookAheadByOneChar(fp) == '/') {
                        fsmState = S_M_COMMENT_FIN;
                    }
                } else if (c == '\n') {
                    break; // S_EOL_COUNT todo
                } else if (c == EOF) {
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS; // missing ending of comment
                }

                break;
            case S_M_COMMENT_FIN:
                if (c == '/') {
                    token->type = TOK_EMPTY;

                    stringDestroy(str);
                    return SUCCESS;
                } else {
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS; // neither nested comment blocks should be there
                }
            }
    }

    return SUCCESS;
}