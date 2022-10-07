/**
 * @file scanner.c
 *
 * @brief Implementation of scanner module for IFJ22
 */

#include "scanner.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INIT_SIZE 8
#define GROWTH 2

#define LEX_ERROR 1

scanner_t * scannerInit(FILE *stream){
    scanner_t * scanner = calloc(1, sizeof(scanner_t));
    if(scanner == NULL){
        fprintf(stderr, "Memory allocation failed: Struct --- scanner.");
        return NULL;
    }

    scanner->state = S_START;
    scanner->stream = stream;
    scanner->currLine = 1;

    scanner->token = tokenInit(scanner);
    if(scanner->token == NULL){
        free(scanner);
        return NULL;
    }

    return scanner;
}

token_t * tokenInit(scanner_t * scanner){
    token_t * currToken = calloc(1, sizeof(token_t));
    if(currToken == NULL){
        fprintf(stderr, "Memory allocation failed: Struct --- token.");
        return NULL;
    }

    currToken->line = scanner->currLine;

    currToken->string = stringInit();
    if(currToken->string == NULL){
        free(currToken);
        return NULL;
    }

    return currToken;
}

string_t * stringInit(){
    string_t * currString = calloc(1, sizeof(string_t));
    if(currString == NULL){
        fprintf(stderr, "Memory allocation failed: Struct --- string.");
        return NULL;
    }

    currString->memSize = INIT_SIZE;

    currString->data = calloc(INIT_SIZE, sizeof(char));
    if(currString->data == NULL){
        fprintf(stderr, "Memory allocation failed: Char* --- data.");
        free(currString);
        return NULL;
    }
    
    return currString;
}

token_list_t * listInit(){
    token_list_t * tokenList = calloc(1, sizeof(token_list_t));
    if(tokenList == NULL){
        fprintf(stderr, "Memory allocation failed: Struct --- TK_list.");
        return NULL;
    }
    
    return tokenList;
}

token_node_t * nodeInit(){
    token_node_t * currNode = calloc(1, sizeof(token_node_t));
    if(currNode == NULL){
        fprintf(stderr, "Memory allocation failed: Struct --- TK_node.");
        return NULL;
    }

    currNode->next = NULL;
    
    return currNode;
}

MachineState transition(MachineState currState, int c){
    switch (currState)  {
    case S_START:
        if(c == ' ') {
            return S_START;

        } else if(c == '\t') {
            return S_START;

        } else if(c == '(') {
            return S_L_PARENTH;

        } else if(c == ')') {
            return S_R_PARENTH;

        } else if(c == ';') {
            return S_SEMICOLON;

        } else if(c == ':') {
            return S_COLON;

        } else if(c == ',') {
            return S_COMA;

        } else if(c == '{') {
            return S_L_BRACE;

        } else if(c == '}') {
            return S_R_BRACE;

        } else if(c == '!') {
            return S_STRT_NEG_COMP;

        } else if (c == '=') {
            return S_ASSIGN;

        } else if (c == '<') {
            return S_LESSER;

        } else if (c == '>') {
            return S_GREATER;

        } else if (c == '+') { 
            return S_ADDITION;

        } else if (c == '-') { 
            return S_SUBTRACT;

        } else if (c == '*') { 
            return S_MULTIPLY;
            
        } else if (c == '.') { 
            return S_CONCAT;
            
        } else if (isdigit(c)) { 
            return S_SUBTRACT;
            
        } else if (c == '"') { 
            return S_STR_LIT;
            
        } else if (c == '-') { 
            return S_SUBTRACT;
            
        }  else if(c == '_' || isalpha(c)) {
            return S_KEYW_OR_ID;

        } else if(c == '?') {
            return S_QSTN_MARK;

        } else if(c == '$') {
            return S_STRT_VAR;

        } else if(c == '/') {
            return S_SLASH;

        } else if(c == '\n') {
            return S_EOL;

        } else if(c == EOF) {
            return S_EOF;

        }
    case S_L_PARENTH:
        return S_START;

    case S_R_PARENTH:
        return S_START;

    case S_SEMICOLON:
        return S_START;

    case S_COLON:
        return S_START;

    case S_COMA:
        return S_START;

    case S_L_BRACE:
        return S_START;

    case S_R_BRACE:
        return S_START;

    case S_ADDITION:
        return S_START;

    case S_SUBTRACT:
        return S_START;

    case S_MULTIPLY:
        return S_START;

    case S_CONCAT:
        return S_START;

    case S_EOL:
        return S_START;

    case S_EOF:
        return S_START;

    case S_STRT_NEG_COMP:
        if(c == '='){return S_MID_NEG_COMP;} 
        return S_ERROR;

    case S_MID_NEG_COMP:
        if(c == '='){return S_NEG_COMP;}
        return S_ERROR;

    case S_ASSIGN:
        if(c == '='){return S_STRT_COMP;}
        return S_END;

    case S_STRT_COMP:
        if(c == '='){return S_COMP;}
        return S_ERROR;

    case S_GREATER:
        if(c == '='){return S_GREATER_EQ;}
        return S_END;

    case S_LESSER:
        if(c == '='){
            return S_LESSER_EQ;

        } else if(c == '?'){
            return S_PROLOGUE;

        } 
        return S_END;

    case S_INT_LIT:
        if(isdigit(c)){
            return S_INT_LIT;

        } else if(c == '.'){
            return S_STRT_DEC;

        } else if(c == 'E' || c == 'e'){
            return S_STRT_EXP;

        }
        return S_END;

    case S_STRT_EXP:
        if(c == '+' || c == '-'){
            return S_MID_EXP;

        } else if(isdigit(c)){
            return S_EXP_LIT;

        }
        return S_ERROR;

    case S_MID_EXP:
        if(isdigit(c)){return S_EXP_LIT;}
        return S_ERROR;

    case S_EXP_LIT:
        if(isdigit(c)){return S_EXP_LIT;}
        return S_END;

    case S_STRT_DEC:
        if(isdigit(c)){return S_DEC_LIT;}
        return S_ERROR;

    case S_DEC_LIT:
        if(isdigit(c)){
            return S_DEC_LIT;

        } else if(c == 'E' || c == 'e'){
            return S_STRT_EXP;

        }
        return S_END;
    case S_STRT_STR:
        if (c == '"'){
            return S_STR_LIT;

        } else if(c == '\\'){
            return S_STRT_ESCP_SQNC;

        } else if(c == EOF){
            return S_ERROR;

        }
        return S_STRT_STR;

    case S_STRT_ESCP_SQNC:
        if (c == 'x'){
            return S_HEX_SCP_SQNC;

        } else if(c < 47 && c > 57){ // [0-8]
            return S_OCT_SCP_SQNC;

        } else if(c == 't' || c == 'n' || c == '"' || c == '$' || c == '\\'){
            return S_SNGL_SCP_SQNC;

        } else if(c == EOF){
            return S_ERROR;

        }
        return S_STRT_STR;

    case S_HEX_SCP_SQNC:
        if(isdigit(c) || (c > 64 && c < 71) || (c > 96 && c < 103)){ // [0-9] [a-f] [A-F]
            return S_HEX_SCP_SQNC;

        } else if(c == EOF){
            S_ERROR;

        }
        return S_STRT_STR;

    case S_OCT_SCP_SQNC:
        if(c < 47 && c > 57){
            return S_OCT_SCP_SQNC;

        } else if(c == EOF){
            return S_ERROR;

        }
        return S_STRT_STR;
    
    case S_KEYW_OR_ID:
        if (isalnum(c) || c == '_'){
            return S_KEYW_OR_ID;

        }
        return S_END;

    case S_QSTN_MARK:
        if(c == '>'){
            return S_END_SIGN;

        } else if((c > 113 && c < 118) || (c > 101 && c < 104) || c == 'i' || c == 'n' || c == 'l' || c == 'o' || c == 'a'){ // in FSM => TypeChar
            return S_TYPE_ID;

        }
        return S_ERROR;

    case S_TYPE_ID:
        if((c > 113 && c < 118) || (c > 101 && c < 104) || c == 'i' || c == 'n' || c == 'l' || c == 'o' || c == 'a'){ // in FSM => TypeChar
            return S_END;
        
        }
    case S_STRT_VAR:
        if (isalpha(c) || '_'){
            return S_VAR_ID;

        }
        return S_ERROR;
        
    case S_VAR_ID:
        if(isalnum(c) || c == '_'){
            return S_VAR_ID;

        }
        return S_ERROR;

    case S_SLASH:
        if(c == '/'){
            return S_S_COMMENT;

        } else if(c == '*'){
            return S_M_COMMENT;

        }
        return S_END;

    case S_S_COMMENT:
        if(c == EOF || c == '\n'){
            return S_S_COMMENT;

        }
        return S_END;

    case S_STRT_M_COMMENT:
        if(c == '*'){
            return S_M_COMMENT_FIN;

        } else if(c == '\n'){
            return S_EOL_COUNT;

        } else if(c == EOF){
            return S_ERROR;

        }
        return S_STRT_M_COMMENT;

    case S_EOL_COUNT:
        if(c == EOF){
            return S_ERROR;

        }
        return S_STRT_M_COMMENT;

    case S_M_COMMENT_FIN:
        if(c == '/'){
            return S_M_COMMENT;

        }
        return S_ERROR;
    }
}

bool resize(string_t * currString){
    size_t oldSize = currString->memSize;
    if((currString->currLen+1) == currString->memSize){
        currString->memSize = currString->memSize * GROWTH;
        void* data = realloc(currString, currString->memSize * sizeof(char));
        
        if(data == NULL){
            return false;
        }else{
            currString->data = data;
        }
    }

    memset(currString->data + oldSize, 0, oldSize);
    return true;
}

int getChar(scanner_t * scanner){
    int c = getc(scanner->stream);
    ungetc(c, scanner->stream);

    return c;
}

bool charPushBack(string_t * currString, int c){
    if(resize(currString) == false){
        return false;
    }

    currString->data[currString->currLen++] = (char)c;
    return true;
}

int checkKeyword(string_t * str, token_t * token) {
    if (!strcmp(str->data, "else")) {
        token->type = TOK_ELSE;
    } else if (!strcmp(str->data, "float")) {
        token->type = TOK_FLOAT;
    } else if (!strcmp(str->data, "function")) {
        token->type = TOK_FUNCTION;
    } else if (!strcmp(str->data, "if")) {
        token->type = TOK_IF;
    } else if (!strcmp(str->data, "int")) {
        token->type = TOK_INT;
    } else if (!strcmp(str->data, "null")) {
        token->type = TOK_NULL;
    } else if (!strcmp(str->data, "return")) {
        token->type = TOK_RETURN;
    } else if (!strcmp(str->data, "string")) {
        token->type = TOK_STRING;
    } else if (!strcmp(str->data, "void")) {
        token->type = TOK_VOID;
    } else if (!strcmp(str->data, "true")) {
        token->type = TOK_TRUE;
    } else if (!strcmp(str->data, "false")) {
        token->type = TOK_FALSE;
    } else if (!strcmp(str->data, "while")) {
        token->type = TOK_WHILE;
    } else if (!strcmp(str->data, "php")) {
        token->type = TOK_KEY_ID;
    } else {
        return 0; // it isn't keyword
    }

    return 1;
}

int get_token(FILE *stream, token_t * token) {// token members -> type, attribute, line, column
    int c = getChar(stream);

    
}
