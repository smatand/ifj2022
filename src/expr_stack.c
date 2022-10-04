/**
 * @file expr_stack.c 
 *
 * @brief Implementation of expression module for IFJ22
 */
#include"expr_stack.h"





void stack_init(stack_t *st){
    st->stack[0]='$'; 
    st->curr_len = 1;
    st->max_len = 5;

}
void stack_push(stack_t *st, char *symbol){
    if(st->curr_len < st->max_len){
        for(int i = 0; i < st->max_len -1 ; i++){

        }
    }
}