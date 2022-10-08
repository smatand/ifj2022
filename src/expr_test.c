#include<stdio.h>
#include<stdlib.h>
#include"expr_stack.h"
#include"expr.h"
// #include"scanner.h"


//docasny generator testovacich tokenov
token_t *myTokenInit(int type){
    token_t *token = malloc(sizeof(token_t));
    token->type = type;
    return token;
}

int main(){
    eStack_t stack;
    eStackInit(&stack);
    eStack_t *stackPtr = &stack;
    eStackPushDollar(stackPtr);
    eStackPushIndent(stackPtr);

    //  token_t *token1 = myTokenInit(TOK_INT_LIT);
    token_t *token1 = myTokenInit(TOK_LEFT_PAREN);
    token_t *token3 = myTokenInit(TOK_RIGHT_PAREN);
    eItem_t *item1 = eItemInit(token1,TERM);  
    eItem_t *item3 = eItemInit(token3,TERM);  

    eStackPushItem(stackPtr, item1);
    eStackPushNonTerm(stackPtr);
    eStackPushItem(stackPtr, item3);
    eStackPrint(stackPtr);
    exprReduce(stackPtr);
    eStackPrint(stackPtr);
    // eItem_t *item3 = eItemInit(token3,TERM);  

    // eStackPushItem(stackPtr,item1);
    // eStackPrint(stackPtr);

    // exprReduce(stackPtr);
    // eStackPrint(stackPtr);

    // token_t *token1 = myTokenInit(TOK_INT_LIT);
    // eItem_t *item1 = eItemInit(token1,TERM);  

    // eStackPushItem(stackPtr,item1);
    // eStackPrint(stackPtr);

    // exprReduce(stackPtr);
    // eStackPrint(stackPtr);


    eStackEmptyAll(stackPtr);
}
