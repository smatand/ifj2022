/**
* @file expr_stack.c
*
* @brief Implementation of parser module for IFJ22
*/

#ifndef EXPR_STACK_H
#define EXPR_STACK_H

#include <stdbool.h>
#include"scanner.h"


typedef enum{
	TERM,
	NONTERM,
	INDENT,	
}eType_t;

typedef struct eItem{
	eType_t type;
	token_t * token;
	struct eItem *next;	
}eItem_t;

typedef struct eStack{
	int currSize;
	eItem_t *head;
}eStack_t;


#endif /* EXPR_STACK_H */ 
