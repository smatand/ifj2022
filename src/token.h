/**
 * @file token.h
 * @author Martin Marsalek - xmarsa15
 *
 * @brief Token header file, includes functions for dealing with sym_table token data
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "symtable.h"
#include "error.h"
#include "str.h"

#define MAX_PARAM_COUNT 20

char *createTokenKey(string_t* ID);

struct token_data *createTokenDataFunction();
struct token_data *createTokenDataVariable();

void functionAddParam(token_data_t *funcData, char *paramID);

#endif
