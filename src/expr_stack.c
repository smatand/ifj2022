/**
 * Project: Translator of language IFJ22
 * @file expr_stack.c
 * @author Tomáš Frátrik - xfratr01
 * 
 * @brief Implementation of a stack for the precedence analyzer unit
 */

#include"expr_stack.h"
#include"expr.h"
#include"error.h"
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>


void eStackInit(eStack_t *stack){
    stack->head = NULL;
    stack->currSize = 0;
}

void eStackEmptyAll(eStack_t *stack){
    while(stack->head != NULL) {
        eStackDeleteFirst(stack);
    }
    stack->head = NULL;
    stack->currSize = 0;
}

eItem_t *eItemInit(token_t *token, int itemType){
    eItem_t *item = malloc(sizeof(struct eItem));
    if(item == NULL){
        return item;
    }
    item->token = token;
    item->type = itemType;
    item->next = NULL;
    item->id = 0;
    return item;
}

void eStackPushItem(eStack_t *stack,eItem_t *item){
    item->next = stack->head;
    stack->head = item;
    stack->currSize++;
}

int eStackPushIndent(eStack_t *stack){
    int ret = SUCCESS;
    eItem_t *indentItem = eItemInit(NULL,INDENT);
    if(indentItem == NULL){
        ret = ERR_INTERNAL;
        return ret;
    }
    if(stack->head->type != NONTERM){
        eStackPushItem(stack,indentItem);
        return SUCCESS;
    }
    eItem_t *poppeNonTerm = eStackPopItem(stack);
    eStackPushItem(stack,indentItem);
    eStackPushItem(stack,poppeNonTerm);
    return SUCCESS; 
}

void eStackPushDollar(eStack_t *stack){
    eItem_t *dollarItem = eItemInit(NULL,DOLLAR);
    eStackPushItem(stack,dollarItem);
}

void eStackPushNonTerm(eStack_t *stack){
    eItem_t *nonTermItem = eItemInit(NULL,NONTERM);
    eStackPushItem(stack,nonTermItem);
}
// void eStackPushNonTerm(eStack_t *stack,token_t *token){
//     eItem_t *nonTermItem = eItemInit(token,NONTERM);
//     eStackPushItem(stack,nonTermItem);
// }

eItem_t *eStackPopItem(eStack_t *stack){
    if(stack->head == NULL){
        fprintf(stderr,"popping from an empty stack\n");
        exit(ERR_INTERNAL);
        return NULL;
    }
    eItem_t *item = stack->head;
    stack->head = item->next;
    stack->currSize--;
    return item;
}

void freeItem(eItem_t *item){
    if(item == NULL) return;
    if(item->token != NULL){
        cleanToken(item->token);
    }
    free(item);
    item = NULL;
}

void eStackDeleteFirst(eStack_t *stack){
    eItem_t *itemToDelete = eStackPopItem(stack);
    if(itemToDelete->token != NULL){
        cleanToken(itemToDelete->token);
    }
     free(itemToDelete);
}

void eStackShift(eStack_t *stack, eItem_t *item){
    eStackPushIndent(stack);
    eItem_t *newItem = eItemInit(item->token,TERM);
    eStackPushItem(stack,newItem);
}

void eStackPrintItem(eItem_t *item){
    // printf("Item:\n");
    if(item == NULL){
        printf("[NULL]");
        return;
    }
    switch(item->type){
        case TERM: 
            printf("%s",tokenTypeToStr(item->token));
            break;
        case NONTERM: 
            // printf("E"); break;    
            printf("E(%ld)",item->id); break;    
        case INDENT: 
            printf("<"); break;
        case DOLLAR:
            printf("$"); break;

    }
}

void stackPrint(eStack_t *stack){
    eItem_t *currItem;
    size_t size = stack->currSize;
    size_t i;
    // printf("\n");
    while(size != 0){
        currItem = stack->head;
        i = 1;
        while(i != size){
            currItem = currItem->next;
            i++;
        }
        eStackPrintItem(currItem);
        size--;
    }
    printf("\n");
}
