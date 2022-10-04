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

#define initSize 16
#define growth 8

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
    size_t oldSize = currString->memSize;
    if((currString->currLen+1) == currString->memSize){
        currString->memSize = currString->memSize + growth;
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

bool charPushBack(string_t currString, int c){
    if(resize(currString) == false){
        return false;
    }

    currString->data[currString->currLen++] = (char)c;
    return true;
}