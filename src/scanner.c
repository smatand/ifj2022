/**
 * @file scanner.c
 *
 * @brief Implementation of scanner module for IFJ22
 */

#include "scanner.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INIT_SIZE 8
#define GROWTH 2

#define LEX_ERROR 1

struct String
{
    char *data;
    size_t currLen;
    size_t memSize; // old size of the string
};

struct Token
{
    TokenType type;
    string_t string;
    int line;
};

struct Scanner
{
    token_t token;
    MachineState state;
    FILE *stream;
    int currLine;
};

struct TK_node
{
    token_t token;
    tk_node_t next;
};

struct TK_list
{
    tk_node_t head;
    tk_node_t tail;
};

scanner_t scanner_init(FILE *stream){
    scanner_t scanner = calloc(1, sizeof(struct Scanner));
    if(scanner == NULL){
        fprintf(stderr, "Memory allocation failed: Struct --- scanner.");
        return NULL;
    }

    scanner->state = S_START;
    scanner->stream = stream;
    scanner->currLine = 1;

    scanner->token = token_init(scanner);
    if(scanner->token == NULL){
        free(scanner);
        return NULL;
    }

    return scanner;
}

token_t token_init(scanner_t scanner){
    token_t currToken = calloc(1, sizeof(struct Token));
    if(currToken == NULL){
        fprintf(stderr, "Memory allocation failed: Struct --- token.");
        return NULL;
    }

    currToken->line = scanner->currLine;

    currToken->string = string_init();
    if(currToken->string == NULL){
        free(currToken);
        return NULL;
    }

    return currToken;
}

string_t string_init(){
    string_t currString = calloc(1, sizeof(struct String));
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

tk_list_t list_init(){
    tk_list_t tokenList = calloc(1, sizeof(struct TK_list));
    if(tokenList == NULL){
        fprintf(stderr, "Memory allocation failed: Struct --- TK_list.");
        return NULL;
    }
    
    return tokenList;
}

tk_node_t node_init(){
    tk_node_t currNode = calloc(1, sizeof(struct TK_node));
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
        if(isspace(c)) {
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
            return S_COMP;
        } else if (c == '<') {
            return S_LESSER;
        } else if (c == '>') {
            return S_GREATER;
        } else if (c == '>=')


        break;

    default:
        break;
    }
}

bool resize(string_t currString){
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

int getChar(scanner_t scanner){
    int c = getc(scanner->stream);
    ungetc(c, scanner->stream);

    return c;
}

bool charPushBack(string_t currString, int c){
    if(resize(currString) == false){
        return false;
    }

    currString->data[currString->currLen++] = (char)c;
    return true;
}

int scan(FILE *stream){
    scanner_t scanner = scanner_init(stream);
    if(scanner == NULL){
        return 99; 
    }
    int c = getChar(scanner);


}