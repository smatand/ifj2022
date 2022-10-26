/**
 * @file scanner.c
 * @author János László Vasík, Andrej Smatana
 *
 * @brief Implementation of scanner module for IFJ22
 */

#include "scanner.h"
#include "error.h"
#include "str.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strcmp()
#include <ctype.h> // isalpha()

#define MAX_KEYWORD_CHARS 9 // 8 + 1 for /0 (function + '\0')

token_t * tokenInit() {
    token_t * token = malloc(sizeof(token_t));

    return token;
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

int convertStringToInt(char * s, int base) {
    char * endPtr;

    int res = strtol(s, &endPtr, base);

    return res;
}

void convertStringToDouble(string_t * s, token_t * token) {
    char * endPtr;

    double res = strtod(s->str, &endPtr);

    token->type = TOK_DEC_LIT;
    token->attribute.decVal = res;
}

int copyString(token_t * token, string_t * str){
    token->attribute.strVal = malloc(str->realLen * sizeof(char)); 
    if (token->attribute.strVal != NULL) {
        memcpy(token->attribute.strVal, str->str, str->realLen);
        return SUCCESS;    
    } else {
        stringDestroy(str);
        return ERR_INTERNAL;
    }
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

int fillStr(string_t * s, token_t * token, FILE * fp) {
    char * buff = calloc(1, MAX_KEYWORD_CHARS);
    if (buff == NULL) {
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
                return ERR_INTERNAL;
            }
        }
    }

    if (c == EOF) {
        free(buff);
        return ERR_LEX_ANALYSIS;
    }

    buff[i] = '\0';

    if (stringResize(s, i + MAX_KEYWORD_CHARS) != SUCCESS) {
        free(buff);
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
                    token->type = TOK_LEFT_PAREN;

                    stringDestroy(str);
                    return SUCCESS;
                } else if(c == ')') {
                    token->type = TOK_RIGHT_PAREN;

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
                    token->type = TOK_LEFT_BRACE;

                    stringDestroy(str);
                    return SUCCESS;
                } else if(c == '}') {
                    token->type = TOK_RIGHT_BRACE;

                    stringDestroy(str);
                    return SUCCESS;
                } else if(c == '!') {
                    fsmState = S_STRT_NEG_COMP;
                } else if (c == '=') {
                    if (lookAheadByOneChar(fp) != '=') {
                        token->type = TOK_ASSIGN;

                        stringDestroy(str);
                        return SUCCESS;
                    }
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

                        c = getc(fp); // move the fp to the next char
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
                    // ".92" or etc are taken as a syntax error
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
                } else if (isdigit(c)) {
                    fsmState = S_INT_LIT;
                    if (charPushBack(str, c) != SUCCESS) {
                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                } else if(c == EOF) {
                    token->type = TOK_EOF;

                    stringDestroy(str);
                    return SUCCESS;
                }
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

                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS;
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

                        stringDestroy(str);
                        return ERR_LEX_ANALYSIS; // prologue was not loaded
                    }
                } 
                break;
            case S_INT_LIT:
                if (isdigit(c)) {
                    if (charPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                } else if (c == '.') {
                    if (charPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_STRT_DEC;
                } else if (c == 'E' || c == 'e') {
                    if (charPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_STRT_EXP;
                } else {
                    int res = convertStringToInt(str->str, 10);

                    token->type = TOK_INT_LIT;
                    token->attribute.intVal = res;

                    ungetc(c, fp); // return last read char to stream if it's not 'E', 'e', '.', or a number

                    stringDestroy(str);
                    return SUCCESS;
                }
                break;
            case S_STRT_EXP:
                if (c == '+' || c == '-') {
                    if (charPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_MID_EXP;
                    break;
                } else if (isdigit(c)) {
                    if (charPushBack(str, c) != SUCCESS) {

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
                    if (charPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }

                    fsmState = S_EXP_LIT;
                } else {
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS; // S_ERROR, missing number
                }
                break;
            case S_EXP_LIT:
                if (isdigit(c)) {
                    if (charPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                } else if (isspace(c)) {
                    convertStringToDouble(str, token); // uses strdol() function, which works with exponents

                    stringDestroy(str);
                    return SUCCESS;
                } else {

                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS;
                }
                break;
            case S_STRT_DEC:
                if (isdigit(c)) {
                    if (charPushBack(str, c) != SUCCESS) {

                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_DEC_LIT;
                    break;
                } else {
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS;
                }
            case S_DEC_LIT:
                if (isdigit(c)) {
                    if (charPushBack(str, c) != SUCCESS) {

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
                    token->attribute.strVal = str->str;
                    stringDestroy(str);
                    return SUCCESS;
                } else if (c == '\\') {
                    fsmState = S_STRT_ESCP_SQNC;
                } else if(c == '$') { // dollar sign is not allowed without a backslash before it
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS;
                } else if (c == EOF) {
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS;
                } else { // else it is part of string literal
                    if (charPushBack(str, c) != SUCCESS) {
                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_STRT_STR;
                }
                break;
            case S_STRT_ESCP_SQNC:; // necessary ';', so the declaration of escpStr is valid, and not flagged by gcc
                char escpStr[5] = {'\\', '\0', '\0', '\0', '\0'}; // escape sequence buffer
                    if (c == 'x') { // hexa escape sequence
                        escpStr[1] = c;
                        fsmState = S_HEX_SCP_SQNC;
                        break;
                    } else if (c > 47 && c < 57) { // [0-8] octal escape seq
                        escpStr[1] = c;
                        fsmState = S_OCT_SCP_SQNC;
                        break;
                    } else if (c == 't') { // single char escape sequences
                        if (charPushBack(str, '\t') != SUCCESS) { 
                            stringDestroy(str);
                            return ERR_INTERNAL;
                        }
                        fsmState = S_STRT_STR;
                        break;
                    } else if (c == 'n') {
                        if (charPushBack(str, '\n') != SUCCESS) { 
                            stringDestroy(str);
                            return ERR_INTERNAL;
                        }
                        fsmState = S_STRT_STR;
                        break;
                    } else if (c == '"') {
                        if (charPushBack(str, '"') != SUCCESS) { 
                            stringDestroy(str);
                            return ERR_INTERNAL;
                        }
                        fsmState = S_STRT_STR;
                        break;
                    } else if (c == '$') {
                        if (charPushBack(str, '$') != SUCCESS) { 
                            stringDestroy(str);
                            return ERR_INTERNAL;
                        }
                        fsmState = S_STRT_STR;
                        break;
                    } else if (c == '\\'){
                        if (charPushBack(str, '\\') != SUCCESS) { 
                            stringDestroy(str);
                            return ERR_INTERNAL;
                        }
                        fsmState = S_STRT_STR;
                        break;
                    } else if (c == EOF) {
                        stringDestroy(str);
                        return ERR_LEX_ANALYSIS;
                    }
                    if (charPushBack(str, c) != SUCCESS) { // if no valid escapes were read, treat it as part of the string
                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_STRT_STR;
                    break;
            case S_HEX_SCP_SQNC:
                if (isdigit(c) || (c > 64 && c < 71) || (c > 96 && c < 103)) { // [0-9] [a-f] [A-F] first hexa char
                    escpStr[2] = c;
                    int temp = lookAheadByOneChar(fp);
                    if (isdigit(temp) || (temp > 64 && temp < 71) || (temp > 96 && temp < 103)) { // second hexa char
                        escpStr[3] = temp;
                        escpStr[0] = '0'; // making a convertible format
                        temp = convertStringToInt(escpStr, 16);
                        escpStr[0] = '\\'; // in case of pushBack, it'll be in the original form
                        if (temp > 31 && temp < 127) { // if convertible to a printable and allowed char, will do so
                            if (charPushBack(str, temp) != SUCCESS) {
                                stringDestroy(str);
                                return ERR_INTERNAL;
                            }
                            fsmState = S_STRT_STR;
                            break;
                        } else { // else append the escape sequence to the end of the string literal
                            if(strPushBack(str, escpStr, 4) != SUCCESS){
                                stringDestroy(str);
                                return ERR_INTERNAL;
                            }
                            fsmState = S_STRT_STR;
                            break;
                        }
                    } else if (c == EOF){ // error caused by EOF
                        stringDestroy(str);
                        return ERR_LEX_ANALYSIS;
                    } else { // incorrect hexa escp. seq. -> part of string
                        if(strPushBack(str, escpStr, 4) != SUCCESS){
                            stringDestroy(str);
                            return ERR_INTERNAL;
                        }
                        fsmState = S_STRT_STR;
                        break;
                    }
                } else if (c == EOF){
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS;
                } else {
                    if(strPushBack(str, escpStr, 3) != SUCCESS){// pushing back "\x[invalid hexa char]"
                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_STRT_STR;
                    break;
                }
            case S_OCT_SCP_SQNC:
                if (c > 47 && c < 57) { // checking octal number
                    escpStr[2] = c;
                    int temp = lookAheadByOneChar(fp);
                    if(temp > 47 && temp < 57){ // checking octal number
                        escpStr[3] = temp;
                        escpStr[0] = '0'; // making a convertible format
                        temp = convertStringToInt(escpStr, 8);
                        escpStr[0] = '\\'; // in case of pushBack, it'll be in the original form
                        if(temp > 31 && temp < 127){ // if convertible to a printable and allowed char, will do so
                            if (charPushBack(str, temp) != SUCCESS) {
                                stringDestroy(str);
                                return ERR_INTERNAL;
                            }
                            fsmState = S_STRT_STR;
                            break;
                        } else { // else append the escape sequence to the end of the string literal
                            if(strPushBack(str, escpStr, 4) != SUCCESS){
                                stringDestroy(str);
                                return ERR_INTERNAL;
                            }
                            fsmState = S_STRT_STR;
                            break;
                        }
                    } else if(temp == EOF){
                        if(strPushBack(str, escpStr, 4) != SUCCESS){
                            stringDestroy(str);
                            return ERR_INTERNAL;
                        }
                        strcpy(token->attribute.strVal, str->str);
                        stringDestroy(str);
                        return ERR_LEX_ANALYSIS;
                    } else {
                        if(strPushBack(str, escpStr, 4) != SUCCESS){// pushing back "\[0-8][0-8][not octal char]"
                            stringDestroy(str);
                            return ERR_INTERNAL;
                        }
                        fsmState = S_STRT_STR;
                        break;
                    }
                } else if(c == EOF) {
                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS;
                } else {
                    if(strPushBack(str, escpStr, 3) != SUCCESS){// pushing back "\[0-8][not octal char]"
                        stringDestroy(str);
                        return ERR_INTERNAL;
                    }
                    fsmState = S_STRT_STR;
                    break;
                }
            case S_KEYW_OR_ID:
                ungetc(c, fp); // again to manipulate with c correctly
                ret = fillStr(str, token, fp);

                if (ret == ERR_INTERNAL) {

                    stringDestroy(str);
                    return ERR_INTERNAL;
                } else if (ret == ERR_LEX_ANALYSIS) {

                    stringDestroy(str);
                    return ERR_LEX_ANALYSIS;
                }

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

                ret = fillStr(str, token, fp);

                if (ret == ERR_INTERNAL) {

                    stringDestroy(str);
                    return ERR_INTERNAL;
                } else if (ret == ERR_LEX_ANALYSIS) {

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
                    break;
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

    stringDestroy(str);
    return SUCCESS;
}