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
char ownScanner(){
    int c = fgetc(stdin);
    return c;
}

int charToToken(char s){
    switch(s){
        case 'i':
            return TOK_INT_LIT;
        case '+':
            return TOK_PLUS;
        case '-':
            return TOK_MINUS;
        case '*':
            return TOK_STAR;
        case '(':
            return TOK_LEFT_PAREN;
        case ')':
            return TOK_RIGHT_PAREN;
        case '$':
            return 100;
    }
}

int main(){
    bool continueParsing = true;
	eStack_t estack;
	eStack_t *stack = &estack;
	eStackInit(stack);
	eStackPushDollar(stack);





    int incomingTokenType;
    int stackTokenType;
    int incomingTokenType_newType;
    int c;
    eItem_t *newItem;
    token_t *newToken;
    bool scan = true;
    // stackPrint(stack);
    while(true){
        if(scan){
            c = ownScanner();
        }
        // printf("char: %c\n",c);
        scan = true;
        incomingTokenType = charToToken(c);
        if(incomingTokenType != 100){
            newToken = myTokenInit(incomingTokenType);
            incomingTokenType_newType = tokenTypeToeType(newToken);
            newItem = eItemInit(newToken,TERM);
        }
        else{
            newToken = myTokenInit(TOK_COMMA);
            newItem = eItemInit(newToken,DOLLAR);
            incomingTokenType_newType = P_DOLLAR;
            // printf("found dollar\n");
        }
        // eStackPrintItem(newItem);

        eItem_t *closestTerm = findClosestTerm(stack);
        if(closestTerm->type == DOLLAR){
            stackTokenType = P_DOLLAR;
        }
        else{
            stackTokenType = tokenTypeToeType(closestTerm->token);
        }
        char operation = precedenceTable[stackTokenType][incomingTokenType_newType];
        
        switch(operation){
            case '<':
                eStackShift(stack,newItem);
                break;
            case '>':
                exprReduce(stack);
                break;
            case '=':
                // newItem = eItemInit(newToken TERM);
                eStackPushItem(stack,newItem);
                break;
            case '!':
                printf("ERROR\n");
                break;
            default:
                printf("SWITCH error\n");
        }
        if(newItem->type == DOLLAR){
            // exprReduce(stack);  
            break;
        }
        if(operation == '>'){
            scan = false;
        }
        // eStackPrint(expr);
        stackPrint(stack);
    }
        // printf("tu som \n");
        stackPrint(stack);
    while(stack->head->next->type != DOLLAR){
        exprReduce(stack);
        stackPrint(stack);
    }

    eStackEmptyAll(stack);

}
        // eStackPrint(stack);
    // int stackTokenType;
    // int currItemToken;
    // eItem_t *curritem = expr->head;
    // eItem_t *newItem;
    // printf("example: (i+i)*i\n");
    // stackPrint(stack);
    // while(true){
    //     eItem_t *closestTerm = findClosestTerm(stack);

    //     if(closestTerm->type == DOLLAR){
    //         stackTokenType = P_DOLLAR;
    //     }
    //     else{
    //         stackTokenType = tokenTypeToeType(closestTerm->token);
    //     }
    //     // eStackPrintItem(curritem);
    //     if(curritem->type == DOLLAR){
    //         currItemToken = P_DOLLAR;
    //     }
    //     else{
    //         currItemToken = tokenTypeToeType(curritem->token);

    //     }
    //     // printf("TEST\n");
    //     // if(currItemToken == P_DOLLAR) printf("MAM DOLLAR\n");
    //     char operantion = precedenceTable[stackTokenType][currItemToken];
        
    //     switch(operantion){
    //         case '<':
    //             eStackShift(stack,curritem);
    //             break;
    //         case '>':
    //             exprReduce(stack);
    //             break;
    //         case '=':
    //             newItem = eItemInit(curritem->token, TERM);
    //             eStackPushItem(stack,newItem);
    //             break;
    //         case '!':
    //             printf("ERROR\n");
    //             break;
    //         default:
    //             printf("SWITCH error\n");
    //     }
    //     if(curritem->type == DOLLAR){
    //         break;
    //     }
    //     if(operantion != '>'){
    //         curritem = curritem->next;
    //     }
    //     // eStackPrint(expr);
    //     stackPrint(stack);
    // }
    //     stackPrint(stack);
    // while(stack->head->next->type != DOLLAR){
    //     exprReduce(stack);
    //     stackPrint(stack);
    //     // eStackPrint(stack);
    // }

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
