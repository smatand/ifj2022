#include <stdlib.h>

#include "sym_table_stack.h"

#define INITIAL_BUCKET_COUNT 113

int init_stack(sym_stack_t **stack)
{
	// dynamically allocate memory for the stack
	*stack = calloc(sizeof(sym_stack_t), 1);
	if (*stack == NULL)
	{
		return ERR_INTERNAL;
	}

	return SUCCESS;
}

void push_table(sym_stack_t *stack, htab_t *table)
{
	sym_stack_element_t *new = malloc(sizeof(sym_stack_element_t));
	if (new == NULL)
	{
		fprintf(stderr, "[ERROR] Sym table stack, failure to allocate element in push function");
		exit(ERR_INTERNAL);
	}

	new->table = table;
	new->next = stack->top;
	stack->top = new;
}

void push_empty(sym_stack_t *stack)
{
	sym_stack_element_t *new = malloc(sizeof(sym_stack_element_t));
	if (new == NULL)
	{
		fprintf(stderr, "[ERROR] Sym table stack, failure to allocate element in push function");
		exit(ERR_INTERNAL);
	}

	new->table = htab_init(INITIAL_BUCKET_COUNT);
	new->next = stack->top;
	stack->top = new;
}

int pop(sym_stack_t *stack)
{
	if (stack->top == NULL) {
		return 1;
	}

	sym_stack_element_t *tmp = stack->top;
	stack->top = tmp->next;
	htab_free(tmp->table);
	free(tmp);


	return 0;
}

htab_t *top(sym_stack_t *stack)
{
	if(stack->top != NULL)
	{
		return stack->top->table;
	}

	return NULL;
}

bool is_empty(sym_stack_t *stack)
{
	return (stack->top == NULL);
}

void empty_stack(sym_stack_t *stack)
{
	while (!pop(stack)) ;

	free(stack);
}
