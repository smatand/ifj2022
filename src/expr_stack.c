/**
 * @file expr_stack.c 
 *
 * @brief Implementation of parser module for IFJ22
 */
#include"expr_stack.h"
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>

//i+i*i
//$
//i > $
//E$

void eStackInit(eStack_t *stack){
    //error
    stack->head = NULL;
    int currSize = 0;
}

//dostane param token a tak a to sa mallocne

void eStackEmptyAll(eStack_t *stack){
    while(stack->head != NULL) {
        eStackDeleteFirst(stack);
    }
}

eItem_t *eItemInit(token_t *token, int itemType){
    eItem_t *item = malloc(sizeof(struct eItem));
    //malloc error
    item->token = token;
    item->type = itemType;
    item->next = NULL;
    return item;
}


void eStackPushItem(eStack_t *stack,eItem_t *item){
    item->next = stack->head;
    stack->head = item;
    stack->currSize++;
}

eItem_t *eStackPopItem(eStack_t *stack){
    if(stack->head == NULL){
        fprintf(stderr,"popping from empty stack\n");
        return NULL;
    }
    eItem_t *item = stack->head;
    stack->head = item->next;
    if(item != NULL){
        stack->currSize--;
    }
    return item;
}

void eStackDeleteFirst(eStack_t *stack){
    eItem_t *itemToDelete = eStackPopItem(stack);
    free(itemToDelete);
}
	// eType_t type;
	// token_t *token;
	// struct eItem *next;	
void eStackPrintItem(eItem_t *item){
    printf("\n");
    printf("Item:\n");
    if(item == NULL){
        printf("NULL\n");
        return;
    }
    printf(" etype %d\n",item->type);
    // printf(" tokentype: %d\n",item->token->type);
    printf(" [->%s]\n",(item->next) ? "*" : "NULL");
}

void eStackPrint(eStack_t *stack){
    eItem_t *item = stack->head;
    printf("\nStart:\n");
    while(item != NULL){
        // // printf("[");
        // if(item->type == INDENT){
        //     printf("[<] -> ");
        // }
        // // else if(item->type == TERM){
        // //     printf("[TERM:%d] -> ",item->token->type);
        // // }
        // else if(item->type == NONTERM){
        //     printf("[NONTERM] -> ");
        // }
        // else{
        //     printf("ERROR -> ");
        // }
        eStackPrintItem(item);
        item = item->next;
    }
}

