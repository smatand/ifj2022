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

token_data_t *createTokenDataFunction(char *ID); // param_list must be created first
token_data_t *createTokenDataVariable(char *ID);

#endif