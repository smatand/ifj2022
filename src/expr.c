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




void exprReduce(eStack_t *stack){
	int currState = E_STATE_START;
	bool repeat = true;
	int tokenType;
	eItem_t *currItem;
	while(repeat){
		switch(currState){
			//starting state
			case E_STATE_START:
				currItem = eStackPopItem(stack);
				if(currItem->type == TERM){		
					//E -> i	
					tokenType = tokenTypeToeType(currItem->token);
					if(tokenType ==  P_RIGHT_PAREN){
						currState = RULE2_EXPECTED1;
						break;
					}
						currState = RULE3_EXPECTED1;
				}
				if(currItem->type == NONTERM){
					//E -> E operand E, for instance: E -> E*E
					currState = RULE1_EXPECTED1;
				}
				break;
				//we fount nonterm -> E on top of stack
			//rule: E->E*E
			case RULE1_EXPECTED1:
				//next token needs to be operand
				free(currItem);
				currItem = eStackPopItem(stack);
				tokenType = tokenTypeToeType(currItem->token);
				if(tokenType != P_ID || tokenType != P_LEFT_PAREN || tokenType != P_RIGHT_PAREN){
					currState = RULE1_EXPECTED2;
				}
				break;
			//rule: E->E*E
			case RULE1_EXPECTED2:
				free(currItem);
				currItem = eStackPopItem(stack);
				if(currItem->type == NONTERM){
					currState = RULE1_EXPECTED3;
				}
				break;
			//rule: E->E*E
			case RULE1_EXPECTED3:
				free(currItem);
				currItem = eStackPopItem(stack);
				if(currItem->type == INDENT){
					printf("3: E -> E*E\n");
					free(currItem);
					eStackPushNonTerm(stack);
					repeat = false;
					return;
				}
				break;
			//rule E -> (E)
			case RULE2_EXPECTED1:
				free(currItem);
				currItem = eStackPopItem(stack);
				if(currItem->type == NONTERM){
					currState = RULE2_EXPECTED2;
				}
				break;
			//rule E -> (E)
			case RULE2_EXPECTED2:
				free(currItem);
				currItem = eStackPopItem(stack);
				tokenType = tokenTypeToeType(currItem->token);
				if(tokenType ==  P_LEFT_PAREN){
					currState = RULE2_EXPECTED3;
				}
			case RULE2_EXPECTED3:
				free(currItem);
				currItem = eStackPopItem(stack);
				if(currItem->type == INDENT){
					printf("2: E -> (E)\n");
					free(currItem);
					eStackPushNonTerm(stack);
					repeat = false;
					return;
				}

			//rule E -> i
			case RULE3_EXPECTED1:
				tokenType = tokenTypeToeType(currItem->token);
				free(currItem);
				if(tokenType == P_ID){
					currItem = eStackPopItem(stack);
					if(currItem->type == INDENT){//todo else error aj u ostatnych
						printf("3: E -> i\n");
						free(currItem);
						eStackPushNonTerm(stack);
						repeat = false;
						return;
					}
				}


		}
	}
}
//todo co patri pod id  co moze byt vo vyraze?
precTokenType_t tokenTypeToeType(token_t *token){
	tokenType_t type = token->type;
	if(type == TOK_KEYWORD){
		keyword_t keyword = token->attribute.kwVal;
		switch(keyword){
			case KW_TRUE:
			case KW_FALSE:
			case KW_NULL:
				return P_ID;
			default:
				return P_ERROR;
		}
	}
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
		case TOK_STRING_LIT:            
    	case TOK_INT_LIT:             
    	case TOK_DEC_LIT:                
			return P_ID;
		default:
			return P_ERROR;
	}
}


char *tokenTypeToStr(token_t *token){
	tokenType_t type = token->type;
	if(type == TOK_KEYWORD){
		keyword_t keyword = token->attribute.kwVal;
		switch(keyword){
			case KW_TRUE:
			case KW_FALSE:
			case KW_NULL:
				return "i";
			default:
				return "error";
		}
	}
    switch(type){
		case TOK_STAR:
			return "*";
		case TOK_PLUS:
			return "+";
		case TOK_MINUS:
			return "-";
		case TOK_SLASH:
			return "/";
		case TOK_DOT:
			return ".";
		case TOK_LESS:
			return "<";
		case TOK_GREATER:
			return ">";
		case TOK_GREATER_EQUAL:
			return ">=";
		case TOK_LESS_EQUAL:
			return "<=";
		case TOK_COMPARISON:
			return "===";
		case TOK_NEG_COMPARISON:
			return "!==";
		case TOK_ASSIGN:
			return "=";
		case TOK_LEFT_PAREN:
			return "(";
		case TOK_RIGHT_PAREN:
			return ")";
		case TOK_SEMICOLON:
			return ";";
		case TOK_STRING_LIT:            
    	case TOK_INT_LIT:             
    	case TOK_DEC_LIT:                
			return "i";
		default:
			return "error";
    }
}