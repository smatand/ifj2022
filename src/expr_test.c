#include<stdio.h>
#include<stdlib.h>
#include"expr_stack.h"
#include"expr.h"
#include"scanner.h"


//docasny generator testovacich tokenov
token_t *myTokenInit(int type){
    token_t *token = malloc(sizeof(token_t));
    token->type = type;
    return token;
}

//i+i*i
//i
eItem_t *findClosestTerm(eStack_t *stack){
    eItem_t *currItem = stack->head;
    while(currItem->type == INDENT || currItem->type == NONTERM){
        currItem = currItem->next;
    }
    return currItem;
}
int main(){
    bool continueParsing = true;
	eStack_t estack;
	eStack_t *stack = &estack;
	eStackInit(stack);
	eStackPushDollar(stack);
    eStack_t expr1;
    eStack_t *expr = &expr1;
    eStackInit(expr);


    //tokens for i+i*i
    token_t *token1 = myTokenInit(TOK_INT_LIT); //1i
    token_t *token2 = myTokenInit(TOK_STAR); //1i
    token_t *token3 = myTokenInit(TOK_INT_LIT); //1i
    token_t *token4 = myTokenInit(TOK_PLUS); //1i
    token_t *token5 = myTokenInit(TOK_INT_LIT); //1i
    eItem_t *item1 = eItemInit(token1, TERM);
    eItem_t *item2 = eItemInit(token2, TERM);
    eItem_t *item3 = eItemInit(token3, TERM);
    eItem_t *item4 = eItemInit(token4, TERM);
    eItem_t *item5 = eItemInit(token5, TERM);
    eStackPushDollar(expr);
    eStackPushItem(expr,item1);
    eStackPushItem(expr,item2);
    eStackPushItem(expr,item3);
    eStackPushItem(expr,item4);
    eStackPushItem(expr,item5);

    int stackTokenType;
    int currItemToken;
    eItem_t *curritem = expr->head;
    // eStackPrint(expr);
    // eItem_t *newitem;
    while(true){
        // eStackPrint(expr);
        eStackPrint(stack);
        eItem_t *closestTerm = findClosestTerm(stack);

        if(closestTerm->type == DOLLAR){
            stackTokenType = P_DOLLAR;
        }
        else{
            stackTokenType = tokenTypeToeType(closestTerm->token);
        }
        eStackPrintItem(curritem);
        if(curritem->type == DOLLAR){
            currItemToken = P_DOLLAR;
        }
        else{
            currItemToken = tokenTypeToeType(curritem->token);

        }
        // printf("TEST\n");
        // if(currItemToken == P_DOLLAR) printf("MAM DOLLAR\n");
        char operantion = precedenceTable[stackTokenType][currItemToken];
        
        switch(operantion){
            case '<':
                printf("SHIFT\n");
                eStackShift(stack,curritem);
                break;
            case '>':
                printf("REDUCE\n");
                exprReduce(stack);
                break;
            case '=':
                printf("EQUAL\n");
                eItem_t *newItem = eItemInit(curritem->token, TERM);
                eStackPushItem(stack,newItem);
                break;
            case '!':
                printf("ERROR\n");
                break;
            default:
                printf("SWITCH error\n");
        }
        if(curritem->type == DOLLAR){
            break;
        }
        if(operantion != '>'){
            curritem = curritem->next;
        }
        // eStackPrint(expr);
    }

    while(stack->head->next->type != DOLLAR){
        exprReduce(stack);
        eStackPrint(stack);
    }

		
    eStackEmptyAll(stack);
    // eStack_t stack;
    // eStackInit(&stack);
    // eStack_t *stackPtr = &stack;
    // eStackPushDollar(stackPtr);
    // eStackPushIndent(stackPtr);

    // //  token_t *token1 = myTokenInit(TOK_INT_LIT);
    // token_t *token1 = myTokenInit(TOK_LEFT_PAREN);
    // token_t *token3 = myTokenInit(TOK_RIGHT_PAREN);
    // eItem_t *item1 = eItemInit(token1,TERM);  
    // eItem_t *item3 = eItemInit(token3,TERM);  

    // eStackPushItem(stackPtr, item1);
    // eStackPushNonTerm(stackPtr);
    // eStackPushItem(stackPtr, item3);
    // eStackPrint(stackPtr);
    // exprReduce(stackPtr);
    // eStackPrint(stackPtr);
    // // eItem_t *item3 = eItemInit(token3,TERM);  

    // // eStackPushItem(stackPtr,item1);
    // // eStackPrint(stackPtr);

    // // exprReduce(stackPtr);
    // // eStackPrint(stackPtr);

    // // token_t *token1 = myTokenInit(TOK_INT_LIT);
    // // eItem_t *item1 = eItemInit(token1,TERM);  

    // // eStackPushItem(stackPtr,item1);
    // // eStackPrint(stackPtr);

    // // exprReduce(stackPtr);
    // // eStackPrint(stackPtr);


    // eStackEmptyAll(stackPtr);
}
