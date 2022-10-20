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
        case '/':
            return TOK_SLASH;
        case '.':
            return TOK_DOT;
        case ';':
            return 100;
        default:
            return 99;
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
    eItem_t *newItem = NULL;
    token_t *newToken = NULL;
    bool scan = true;
    bool exit = false;
    while(true){
        if(scan){
            c = ownScanner();
            incomingTokenType = charToToken(c);
            if(incomingTokenType == TOK_INT_LIT){
            }
            if(incomingTokenType != 100){
                newToken = myTokenInit(incomingTokenType);
                incomingTokenType_newType = tokenTypeToeType(newToken);
                newItem = eItemInit(newToken,TERM);
            }
            else{
                newToken = myTokenInit(TOK_SEMICOLON);
                newItem = eItemInit(newToken,DOLLAR);
                incomingTokenType_newType = P_DOLLAR;
            }
        }
        scan = true;

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
                exprShift(stack,newItem);
                break;
            case '>':
                exprReduce(stack);
                break;
            case '=':
                eStackPushItem(stack,newItem); 
                break;
            case '!':
            // printf("trying recovery\n");
                if(incomingTokenType_newType == P_RIGHT_PAREN){
                    while(stack->head->next->type != DOLLAR){
                        exprReduce(stack);
                        stackPrint(stack);
                    }
                    exit = true;
                    //staci return )
                    break;
                }
                printf("ERROR\n");
                break;
            default:
                printf("SWITCH error\n");
        }
        if(newItem->type == DOLLAR || exit){
            freeItem(newItem);
            break;
        }
        if(operation == '>'){
            scan = false;
        }
        stackPrint(stack);
    }
    if(stack->head->next->type != DOLLAR){
            stackPrint(stack);
        while(stack->head->next->type != DOLLAR){
            exprReduce(stack);
            stackPrint(stack);
        }
    }

    eStackEmptyAll(stack);

}
