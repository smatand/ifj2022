/**
 * @file token.h
 *
 * @brief Token header file, includes functions for dealing with sym_table token data
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "symtable.h"
#include "error.h"

#define BASEPARAMLISTSIZE 4 // base size of data_type_t array to allocate for param_list_t

param_list_t* createParamList();
void addToParamList(param_list_t *param_list, data_type_t data_type);
void destroyParamList(param_list_t * param_list);

token_data_t *createTokenDataFunction(char *ID, param_list_t* param_list, data_type_t return_type, bool defined); // param_list must be created first
token_data_t *createTokenDataVariable(char *ID, data_type_t type);

token_data_t *createTokenDataConstantInt(char *ID, int value);
token_data_t *createTokenDataConstantFloat(char *ID, float value);
token_data_t *createTokenDataConstantString(char *ID, char *value);
token_data_t *createTokenDataConstantNull(char *ID);

#endif