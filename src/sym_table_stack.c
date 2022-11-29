#include "sym_table_stack.h"

void init_stack(sym_stack_t *stack)
{
	stack->top = NULL;
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
}

bool is_empty(sym_stack_t *stack)
{
	return (stack->top == NULL);
}

void destroy_stack(sym_stack_t *stack)
{
	// TODO?
}