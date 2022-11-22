/**
 * @file token.h
 *
 * @brief Token header file, includes functions for dealing with sym_table tokens
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "symtable.h"
#include "error.h"

token_data_t *createTokenDataFunction(char *ID, data_type_t* param_list, int param_count, data_type_t return_type, bool defined);
token_data_t *createTokenDataVariable(char *ID, data_type_t type);

token_data_t *createTokenDataConstantInt(char *ID, int value);
token_data_t *createTokenDataConstantFloat(char *ID, float value);
token_data_t *createTokenDataConstantString(char *ID, char *value);
token_data_t *createTokenDataConstantNull(char *ID);

#endif