/**
 * @file token.h
 *
 * @brief Token header file, includes functions for dealing with sym_table token data
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "symtable.h"
#include "error.h"
#include "str.h"

#define BASE_PARAM_LIST_SIZE 4 // base size of data_type_t array to allocate for param_list_t
#define TOKEN_KEY_SPECIFIER_SIZE 2 // amount of extra characters in token key

char *createTokenKey(string_t* ID);

// param_list_t* createParamList();
// void addToParamList(param_list_t *param_list, data_type_t data_type, string_t* name);
// void destroyParamList(param_list_t * param_list);

token_data_t *createTokenDataFunction(char *ID); // param_list must be created first
token_data_t *createTokenDataVariable(char *ID);

// token_data_t *createTokenDataConstantInt(char *ID, int value);
// token_data_t *createTokenDataConstantFloat(char *ID, float value);
// token_data_t *createTokenDataConstantString(char *ID, char *value);
// token_data_t *createTokenDataConstantNull(char *ID);

#endif