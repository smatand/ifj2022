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

char *createTokenKey(string_t* ID);

struct token_data *createTokenDataFunction(); // param_list must be created first
struct token_data *createTokenDataVariable();

#endif
