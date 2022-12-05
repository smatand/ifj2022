/**
 * Project: Translator of language IFJ22
 * @file sym_table_stack.h
 * @author Martin Maršalek - xmarsa15
 * @author Andrej Smatana - xsmata03
 * 
 * @brief Header file of stack of symbol tables
 */

#ifndef SYM_TABLE_STACK_H
#define SYM_TABLE_STACK_H

#include "symtable.h"
#include "error.h"

#define STACK_MAX_SIZE 100

/** @brief Structure of a stack item's content */
typedef struct sym_stack_element
{
	htab_t *table;
	struct sym_stack_element *next;
} sym_stack_element_t;

/** @brief Structure of a stack of hash tables */
typedef struct sym_stack
{
	sym_stack_element_t *top;
} sym_stack_t;

/** 
 * @brief Initialize stack
 * @param stack pointer to stack to be initialized
 * 
 * @return SUCCESS, otherwise ERR_INTERNAL
 */
int init_stack(sym_stack_t **stack);

/** 
 * @brief Pushes table to the top of stack
 * @param stack pointer to stack to push to
 * @param table pointer to table to push
 * 
 * @return SUCCESS, otherwise ERR_INTERNAL
 */
int push_table(sym_stack_t *stack, htab_t *table);

/** 
 * @brief Pushes empty table to the top of stack
 * @param stack pointer to stack to push to
 * 
 * @return SUCCESS, otherwise ERR_INTERNAL
 */
int push_empty(sym_stack_t *stack);

/** 
 * @brief Pops top table of stack
 * @param stack pointer to stack to pop from
 * 
 * @return SUCCESS (0), otherwise 1
 */
int pop(sym_stack_t *stack);

/** 
 * @brief Returns pointer to top table of stack
 * @param stack pointer to stack struct
 * 
 * @return pointer to top table, otherwise NULL
 */
htab_t *top(sym_stack_t *stack);

/** 
 * @brief Checks if stack is emtpy
 * @param stack pointer to stack struct
 * 
 * @return true if empty, otherwise false
 */
bool is_empty(sym_stack_t *stack);

/** 
 * @brief Completely frees stack struct
 * @param stack pointer to struct
 */
void empty_stack(sym_stack_t *stack);

#endif
