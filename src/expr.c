/**
 * @file expr.c 
 *
 * @brief Implementation of precedence alanysis for IFJ22
 */
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"expr_stack.h"
#include"expr.h"
#include"scanner.h"

const char precedenceTable[TABLE_SIZE][TABLE_SIZE] = {
//  *    +    -    /    .    <    >    >=   <=   ===  !==  =    (    )    i    $ 
  {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},  // *
  {'<', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},  // +
  {'<', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},  // -
  {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},  // /
  {'<', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},  // .
  {'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'},  // <
  {'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'},  // >
  {'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '!', '<', '>', '>', '>'},  // >=
  {'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'},  // <=
  {'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'},  // ===
  {'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'},  // !==
  {'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'},  // =
  {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', '!'},  // (
  {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '!', '>', '!', '>'},  // )
  {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '!', '>', '!', '>'},  // i
  {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '!', '<', '!'},  // $
};


//bude brat parameter token?
//alebo bude si ich sam nacitavat aj s tym prvym
// int exprMain(token_t *token){
//   bool continue = true;
//   while(continue){

//   }
// }

//todo co patri pod id  co moze byt vo vyraze?


precTokenType_t convertToPrecType(TokenType type){
	switch(type){
		case TOK_STAR:
			return P_MUL;
		case TOK_PLUS:
			return P_PLUS;
		case TOK_MINUS:
			return P_MINUS;
		case TOK_SLASH:
			return P_DIV;
		case TOK_DOT:
			return P_DOT;
		case TOK_LESS:
			return P_LESS;
		case TOK_GREATER:
			return P_GREATER;
		case TOK_GREATER_EQUAL:
			return P_GREATER_EQUAL;
		case TOK_LESS_EQUAL:
			return P_LESS_EQUAL;
		case TOK_COMPARISON:
			return P_COMPARISON;
		case TOK_NEG_COMPARISON:
			return P_NEG_COMPARISON;
		case TOK_ASSIGN:
			return P_ASSIGN;
		case TOK_LEFT_PAREN:
			return P_LEFT_PAREN;
		case TOK_RIGHT_PAREN:
			return P_RIGHT_PAREN;
		case TOK_SEMICOLON:
			return P_SEMICOLON;
		case TOK_VAR_ID:
		case TOK_STRING_LIT:            
    	case TOK_INT_LIT:             
    	case TOK_DEC_LIT:                
    	case TOK_EXP_LIT:
		case TOK_TRUE:
		case TOK_FALSE:
		case TOK_NULL:
			return P_ID;
		default:
			return P_ERROR;
	}
}


