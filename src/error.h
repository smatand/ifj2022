/**
* @file error.h
*
* @author Andrej Smatana - xsmata03
*
* @brief Error code definitions
*/

#ifndef ERROR_H
#define ERROR_H

#define SUCCESS               0 // success

#define ERR_LEX_ANALYSIS      1 // error due to lexical analysis
#define ERR_SYN_ANALYSIS      2 // error due to syntactic analysis
#define ERR_SEM               3 // undefined function, redefinition of existing function
#define ERR_SEM_PARAMS        4 // wrong type/amount of parameters when calling function or wrong type of return function
#define ERR_SEM_UNDEFINED_VAR 5 // using undefined variable
#define ERR_SEM_MISSING_EXPR  6 // missing/leftover expression of command in function return statement
#define ERR_SEM_TYPE          7 // type compability of arithmetic, string and relation expressions
#define ERR_SEM_OTHER         8 // other semantic errors

#define ERR_INTERNAL          99 // e.g. memory allocation failed

// alloc/realloc error ----> free all memory, and return ERR_INTERNAL
// other errors -----------> return error code

#endif // ERROR_H