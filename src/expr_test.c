#include<stdio.h>
#include<stdlib.h>
#include"expr_stack.h"
#include"expr.h"
// #include"scanner.h"

int main(){
   eStack_t stack;
   eStackInit(&stack);
   eStack_t *stackPtr = &stack;

   eItem_t *item1 = eItemInit(NULL,TERM);  
   eItem_t *item2 = eItemInit(NULL,TERM);  
//    eStackPrintItem(item1);
//    eItem_t *itemPop = eStackPopItem(stackPtr);
   eStackPushItem(stackPtr,item1);
   eStackPushItem(stackPtr,item2);
//    eStackPushItem(stackPtr,item1);
   eStackPrint(stackPtr);
   eStackEmptyAll(stackPtr);
//    eStackDeleteFirst(stackPtr);
//    eStackPushItem(stack, item1);


}
