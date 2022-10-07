#include<stdio.h>
#include<stdlib.h>
#include"expr_stack.h"
#include"expr.h"
// #include"scanner.h"


// typedef struct {
//     TokenType type;
//     string_t string;
//     int line;

//     // just one of the following will be used
//     union {
//         int intVal;
//         double decVal;
//         char * strVal;
//     } value;
// } token_t;

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

    token_t *token1 = myTokenInit(TOK_STAR);
    eItem_t *item1 = eItemInit(NULL,TERM);  
    eItem_t *item2 = eItemInit(NULL,TERM);  
    eItem_t *item3 = eItemInit(token1,TERM);  
    //    eStackPrintItem(item1);
    //    eItem_t *itemPop = eStackPopItem(stackPtr);
    eStackPushItem(stackPtr,item1);
    eStackPushItem(stackPtr,item2);
    eStackPushItem(stackPtr,item3);
    eStackPushIndent(stackPtr);
    //    eStackPushItem(stackPtr,item1);
    eStackPrint(stackPtr);
    eStackEmptyAll(stackPtr);
//    eStackDeleteFirst(stackPtr);
//    eStackPushItem(stack, item1);


}
