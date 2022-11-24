/**
 * @file sym_table_stack.h
 *
 * @brief Symbol table stack header file
 */

#ifndef SYM_TABLE_STACK_H
#define SYM_TABLE_STACK_H

#include "symtable.h"
#include "error.h"

#define STACK_MAX_SIZE 100

typedef struct sym_stack_element
{
	htab_t *table;
	struct sym_stack_element *next;
} sym_stack_element_t;

typedef struct
{
	sym_stack_element_t *top;
} sym_stack_t;

void init_stack(sym_stack_t *stack);


void push_table(sym_stack_t *stack, htab_t *table);


void push_empty(sym_stack_t *stack);


void pop(sym_stack_t *stack);


htab_t *top(sym_stack_t *stack);


bool is_empty(sym_stack_t *stack);

#endif