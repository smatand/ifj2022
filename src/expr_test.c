#include<stdio.h>
#include<stdlib.h>
#include"expr_stack.h"
#include"expr.h"

int main(){
    printf("digid: %d\n", P_LESS);
    printf("char: >%c<\n",precedence_table[P_MUL][P_MUL]);
    switch(precedence_table[P_LESS][P_LESS]){
        case '!':
            printf("!\n");
            break;
        case '<':
            printf("<\n");
            break;
        case '>':
            printf(">\n");
            break;
        case '=':
            printf("=\n");
            break;
        default:
            printf("ERROR\n");
    }
}
