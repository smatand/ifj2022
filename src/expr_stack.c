/**
 * @file expr_stack.c 
 *
 * @brief Implementation of parser module for IFJ22
 */
#include"expr_stack.h"
#include<stdbool.h>



void eStackInit(eStack_t *stack){
    //error
    stack->head == NULL;
    int currSize = 0;
}

//dostane param token a tak a to sa mallocne
eItem_t *eItemInit(eItem_t *item, token_t *token, eType_t *type){
    eItem_t item = malloc(sizeof(eItem_t));
    //malloc error
    item->token = token;
    item->type = type;
    item->next = NULL;
}


void eStackPushItem(eStack_t * stack,eItem_t *item){
    item->next = stack->head;
    stack->head = item;
}

eItem_t *eStackPopItem(stack_t* stack){
    eItem_t item = stack->head;
    stack->head = item->next;
    return item;
}

void eStackDeleteFirst(stack_t* stack){
    eItem_t itemToDelete = eStackPopItem(stack);
    free(itemToDelete);
}

