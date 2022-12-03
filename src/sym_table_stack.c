#include "sym_table_stack.h"

#define INITIAL_BUCKET_COUNT 113

int init_stack(sym_stack_t *stack)
{
	stack = malloc(sizeof(sym_stack_t));
	if (stack == NULL)
	{
		return ERR_INTERNAL;
	}

	stack->top = NULL;

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

void pop(sym_stack_t *stack)
{
	sym_stack_element_t *temp;
	if (stack->top != NULL)
	{
		temp = stack->top;
		stack->top = stack->top->next;
		free(temp->table);
		free(temp);
	}
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
	while (is_empty(stack) == false)
	{
		pop(stack);
	}
}
