/**
* @file expr_stack.c
*
* @brief Implementation of parser module for IFJ22
*/

#ifndef EXPR_STACK_H
#define EXPR_STACK_H

#include <stdbool.h>
#include"scanner.h"


typedef enum eType{
	TERM,
	NONTERM,
	INDENT,	
	DOLLAR,
}eType_t;

typedef struct eItem{
	eType_t type;
	token_t *token;
	struct eItem *next;	
}eItem_t;

typedef struct eStack{
	int currSize;
	eItem_t *head;
}eStack_t;


void eStackInit(eStack_t *stack);
void eStackEmptyAll(eStack_t *stack);

eItem_t *eItemInit(token_t *token, int itemType);
eItem_t *eStackPopItem(eStack_t *stack);
void eStackPushItem(eStack_t *stack,eItem_t *item);
void eStackPushIndent(eStack_t *stack);
void eStackPushDollar(eStack_t *stack);
void eStackDeleteFirst(eStack_t *stack);
void eStackPushNonTerm(eStack_t *stack);
void stackPrint(eStack_t *stack);

void eStackShift(eStack_t *stack, eItem_t *item);
void eStackPrintItem(eItem_t *item);


void eStackPrint(eStack_t *stack);


#endif /* EXPR_STACK_H */ 


