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

    token_t *token1 = myTokenInit(TOK_INT_LIT);
    eItem_t *item1 = eItemInit(token1,TERM);  

    eStackPushItem(stackPtr,item1);
    eStackPrint(stackPtr);

    exprReduce(stackPtr);
    eStackPrint(stackPtr);


    eStackEmptyAll(stackPtr);
}
