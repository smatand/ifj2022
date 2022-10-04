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

#define initSize 32
#define growth 10

struct String
{
    char *data;
    size_t currLen;
    size_t memSize;
};

string_t string_init(){
    string_t currString = calloc(1, sizeof(struct String));
    if(currString == NULL){
        return NULL;
    }

    currString->data = calloc(initSize, sizeof(char));
    if(currString->data == NULL){
        free(currString);
        return NULL;
    }
    currString->memSize = initSize;
    
    return true;
}

bool resize(string_t currString){
    if((currString->currLen+1) == currString->memSize){
        currString->memSize = currString->memSize + growth;
        void* data = realloc(currString, currString->memSize * sizeof(char));
        
        if(data == NULL){
            return NULL;
        }else{
            currString->data = data;
        }
    }
}

bool stringPushBack(string_t currString, int c){
    currString->data[currString->currLen++] = (char)c;
}