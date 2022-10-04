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

#define initSize 8
#define growth 2

#define lexError 1

struct String
{
    char *data;
    size_t currLen;
    size_t memSize;
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

    currString->memSize = initSize;

    currString->data = calloc(initSize, sizeof(char));
    if(currString->data == NULL){
        fprintf(stderr, "Memory allocation failed: Char* --- data.");
        free(currString);
        return NULL;
    }
    
    return currString;
}

bool resize(string_t currString){
    size_t oldSize = currString->memSize;
    if((currString->currLen+1) == currString->memSize){
        currString->memSize = currString->memSize * growth;
        void* data = realloc(currString, currString->memSize * sizeof(char));
        
        if(data == NULL){
            return false;
        }else{
            currString->data = data;
        }
    }

    memset(currString->data + oldSize, 0, oldSize); // initializing reallocated memory
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
        return NULL;
    }
    int c = getChar(scanner);
}