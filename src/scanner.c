/**
 * @file scanner.c
 *
 * @brief Implementation of scanner module for IFJ22
 */

#include "scanner.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <bool.h>

#define initSize 32

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