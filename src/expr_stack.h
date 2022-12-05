/**
* @file expr_stack.c
*
* @brief Implementation of parser module for IFJ22
*/

#ifndef EXPR_STACK_H
#define EXPR_STACK_H
//test
#include <stdbool.h>
#include"scanner.h"

/** @brief identify item type in stack */
typedef enum eType{
	TERM,
	NONTERM, // E
	INDENT,	// means adding item '<' while shifting
	DOLLAR, //end of stack
}eType_t;

/** @brief single item in stack*/
typedef struct eItem{
	eType_t type;
	token_t *token;
	struct eItem *next;
	size_t id;	
}eItem_t;

/** @brief main stack structure*/
typedef struct eStack{
	int currSize;
	eItem_t *head;
}eStack_t;


/**
 * @brief Initializes given stack
 * 
 * @param stack pointer to stack
 */
void eStackInit(eStack_t *stack);

/**
 * @brief empty all items from stack
 * 
 * @param stack pointer to stack
 */
void eStackEmptyAll(eStack_t *stack);

/**
 * @brief Initialize item
 * 
 * @param token initializing item with given token
 * @param itemType which type from eType_t
 * @return eItem_t* pointer to item
 */
eItem_t *eItemInit(token_t *token, int itemType);

/**
 * @brief pops item from stack and returns it
 * 
 * @param stack pointer to stack
 * @return eItem_t* returns pointer to item
 */
eItem_t *eStackPopItem(eStack_t *stack);
/**
 * @brief pushes item to the top of the stack
 * 
 * @param stack pointer to stack
 * @param item  pointer to item
 */
void eStackPushItem(eStack_t *stack,eItem_t *item);

/**
 * @brief pushes indent = symbol '<', with INDENT type, while shifting
 * 
 * @param stack pointer to stack
 */
int eStackPushIndent(eStack_t *stack);

/**
 * @brief pushes DOLLAR type to the top of stack
 * 
 * @param stack pointer to stack
 */
void eStackPushDollar(eStack_t *stack);

/**
 * @brief pops and frees first item from stack
 * 
 * @param stack pointer to item
 */
void eStackDeleteFirst(eStack_t *stack);

/**
 * @brief pushes NONTERM (E) to the top of stack
 * 
 * @param stack pointer to stack
 * @param token pointer to given token
 */
// void eStackPushNonTerm(eStack_t *stack,token_t *token);
void eStackPushNonTerm(eStack_t *stack);
void stackPrint(eStack_t *stack);

/**
 * @brief frees given item
 * 
 * @param item pointer to item
 */
void freeItem(eItem_t *item);

//prints item -> for debugging
void eStackPrintItem(eItem_t *item);

//prints stack -> for debugging
void eStackPrint(eStack_t *stack);


#endif /* EXPR_STACK_H */ 


