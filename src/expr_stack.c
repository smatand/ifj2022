/**
 * @file expr_stack.c 
 *
 * @brief Implementation of parser module for IFJ22
 */
#include"expr_stack.h"
#include"expr.h"
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>

void eStackInit(eStack_t *stack){
    //error
    stack->head = NULL;
    int currSize = 0;
}

//dostane param token a tak a to sa mallocne

void eStackEmptyAll(eStack_t *stack){
    while(stack->head != NULL) {
        eStackDeleteFirst(stack);
        // eStackPrint(stack);
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

void eStackPushIndent(eStack_t *stack){
    eItem_t *indentItem = eItemInit(NULL,INDENT);
    if(stack->head->type != NONTERM){
        eStackPushItem(stack,indentItem);
        return;
    }
    eItem_t *poppeNonTerm = eStackPopItem(stack);
    eStackPushItem(stack,indentItem);
    eStackPushItem(stack,poppeNonTerm);
}

void eStackPushDollar(eStack_t *stack){
    eItem_t *dollarItem = eItemInit(NULL,DOLLAR);
    eStackPushItem(stack,dollarItem);
}

void eStackPushNonTerm(eStack_t *stack){
    eItem_t *nonTermItem = eItemInit(NULL,NONTERM);
    eStackPushItem(stack,nonTermItem);
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
    //free tokens ?
    if(itemToDelete->token != NULL){
        free(itemToDelete->token);
    }
    free(itemToDelete);
}
void eStackPrintItem(eItem_t *item){
    // printf("Item:\n");
    if(item == NULL){
        printf("[NULL]");
        return;
    }
    switch(item->type){
        case TERM: 
                printf("[%s]",tokenTypeToStr(item->token->type));
                break;
        case NONTERM: 
            printf("[E]"); break;    
        case INDENT: 
            printf("|"); break;
        case DOLLAR:
            printf("$"); break;

    }
}

void eStackPrint(eStack_t *stack){
    eItem_t *item = stack->head;
    printf("\n------------------------------\n");
    while(item != NULL){
        // // printf("[");
        // if(item->type == INDENT){
        //     printf("[<] -> ");
        // }
        // else if(item->type == TERM){
        //     printf("[TERM:%d] -> ",item->token->type);
        // }
        // else if(item->type == NONTERM){
        //     printf("[NONTERM] -> ");
        // }
        // else{
        //     printf("ERROR -> ");
        // }
        eStackPrintItem(item);
        if(item->next == NULL){
            printf(" -> NULL");
        }
        else{
            printf(" -> ");
        }
        item = item->next;
        
    }
        printf("\n------------------------------\n");
}

// void eStackReduce(eStack_t *stack){

// }
