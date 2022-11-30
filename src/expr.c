/**
 * @file expr.c 
 *
 * @brief Implementation of precedence alanysis for IFJ22
 */

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"./expr_stack.h"
#include"./expr.h"
#include"./scanner.h"
#include"./str.h"
#include"./error.h"

//temporary global variable.
size_t nonTermCnt;

int main(){ 
	token_t *token = tokenInit();
	scanToken(token);
	int returnToken;
	int returnVal = exprParse(token,NULL,&returnToken);
	(void)returnToken;
	(void)returnVal;


}

int exprParse(token_t *firstToken, token_t *secondToken, int *returnToken){
	printf(".IFJcode22\n");
	printf("createframe\n");
	printf("pushframe\n");

	int returnVal = SUCCESS;
	nonTermCnt = 0;
	// bool generateCode = true;
	// <: shift with indent
	// >: reduce
	// =: shift without indent
	// !: error
	const char precedenceTable[TABLE_SIZE][TABLE_SIZE] = {
	//*    +    -    /    .    <    >    >=   <=   ===  !==  (    )    i    $ 
	{'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},  // *
	{'<', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},  // +
	{'<', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},  // -
	{'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},  // /
	{'<', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'},  // .
	{'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'},  // <
	{'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'},  // >
	{'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '<', '>', '>', '>'},  // >=
	{'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'},  // <=
	{'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'},  // ===
	{'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'},  // !==
	{'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', '!'},  // (
	{'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '!', '>', '!', '>'},  // )
	{'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '!', '>', '!', '>'},  // i
	{'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '!', '<', '!'},  // $
	};
	if(firstToken == NULL){
		fprintf(stderr,"expr,ERROR: internal Error \n");
		return ERR_SYN_ANALYSIS;
	}
	bool secondTokenDelay = false;
	if(secondToken != NULL){
		secondTokenDelay = true; //expression isn't assigned to anything
	}

	bool continueParsing = true;	
	bool scanAnotherToken = true;

	/* Init stack */
	eStack_t estack;
	eStack_t *stack = &estack; 
	eStackInit(stack);
	eStackPushDollar(stack);

	//initialize
	int stackTokenType;
	int incomingTokenType;
	char operation;
	// (void) returnToken;
	// returnToken = NULL;
	token_t *incomingToken = firstToken;
	eItem_t *closestTerm = NULL;
	eItem_t *incomingTokenItem;
	// stackPrint(stack);

	while(continueParsing){

		if(scanAnotherToken){
			incomingTokenItem = eItemInit(incomingToken,TERM);
			if(incomingTokenItem == NULL){
				freeItem(incomingTokenItem);
				eStackEmptyAll(stack);
				return ERR_INTERNAL;
			}
		}
		incomingTokenType = tokenTypeToeType(incomingToken);
		if(incomingTokenType == P_ERROR){
			freeItem(incomingTokenItem);
			eStackEmptyAll(stack);
			return ERR_INTERNAL;
		}
		closestTerm = findClosestTerm(stack); //closest term in stack

		//if closest term is end of the stack
		if(closestTerm->type == DOLLAR){ 
			stackTokenType = P_DOLLAR;
		}
		else{
			stackTokenType = tokenTypeToeType(closestTerm->token);
			if(stackTokenType == P_ERROR){
				freeItem(incomingTokenItem);
				eStackEmptyAll(stack);
				return ERR_INTERNAL;
			}
		}
		scanAnotherToken = true;


		if(incomingTokenType == P_SEMICOLON){
			operation = '!';
		}
		else{
			//find operation in precedence table
        	operation = precedenceTable[stackTokenType][incomingTokenType];
		}
		switch(operation){
					case '<': //shift with indent
						returnVal=eStackPushIndent(stack);
						if(returnVal != SUCCESS){
							freeItem(incomingTokenItem);
							eStackEmptyAll(stack);
							return returnVal;
						}
						eStackPushItem(stack,incomingTokenItem);
						if(returnVal != SUCCESS){
							freeItem(incomingTokenItem);
							eStackEmptyAll(stack);
							return returnVal;
						}
						break;
					case '>': //reduce
						scanAnotherToken = false;
						returnVal=exprReduce(stack);
						if(returnVal != SUCCESS){
							freeItem(incomingTokenItem);
							eStackEmptyAll(stack);
							return returnVal;
						}
						break;
					case '=': //shift without pushing indent
						eStackPushItem(stack,incomingTokenItem); 
						break;
					case '!': //error or end of expression
						/**
						 * this case still can be correct, if it is right parenthesis ')'
						 * because this could be case where we encounter right closing 
						 * bracket of its statement [example: if(expression')' ],
						 * therefore we send this ending token to top bottom for further analysis
						 * or if it is end of line, finished by semicolon ';'.
						 */
						if(incomingTokenType == P_RIGHT_PAREN || incomingTokenType == P_SEMICOLON){
							//we need to end expression with $E in stack
							while(stack->head->next->type != DOLLAR){ 
								returnVal=exprReduce(stack);
								if(returnVal != SUCCESS){
									freeItem(incomingTokenItem);
									eStackEmptyAll(stack);
									return returnVal;
								}
								// stackPrint(stack);
							}
							continueParsing = false;
							//free last token, ; OR ) and empty whole stack
							freeItem(incomingTokenItem);
							eStackEmptyAll(stack);
							(incomingTokenType == P_RIGHT_PAREN) ? (*returnToken =  TOK_RIGHT_PAREN): (*returnToken =  TOK_SEMICOLON);
							printf("write LF@tmp%ld\n",nonTermCnt);
							printf("popframe\n");
							return returnVal; 
						}

						else{
							freeItem(incomingTokenItem);
							eStackEmptyAll(stack);
							fprintf(stderr,"ERROR: wrong type of token in expression2");
							return ERR_SYN_ANALYSIS;
						}
						break;

					default:
						freeItem(incomingTokenItem);
						eStackEmptyAll(stack);
						fprintf(stderr,"ERROR: wrong type of token in expression3");
						return ERR_SYN_ANALYSIS;
				}

		// stackPrint(stack);
		if(scanAnotherToken){
			if(secondTokenDelay == false){ 
				incomingToken = tokenInit();
				scanToken(incomingToken);
			}
			//this happens only if expression is not assigned to anything
			//this expression is processed if there are no errors, but its result
			//is thrown away	
			else{
				incomingToken = secondToken;
				secondTokenDelay = false;
			}

		}
	}
	printf("popframe\n");
	eStackEmptyAll(stack);
	return SUCCESS;

}


eItem_t *findClosestTerm(eStack_t *stack){
    eItem_t *currItem = stack->head;
    while(currItem->type == INDENT || currItem->type == NONTERM){
        currItem = currItem->next;
    }
    return currItem;
}

int exprReduce(eStack_t *stack){
	int ruleType = exprFindRule(stack);
	eItem_t *currItem = stack->head;
	switch(ruleType){
		case RULE1: //E->E+E, E->E<E, ...
			nonTermCnt++;
			//E
			currItem = eStackPopItem(stack);	
			//oparand
			eItem_t *currItemOperand = eStackPopItem(stack);	
			//E
			eItem_t *currItem3 = eStackPopItem(stack);	
			printf("defvar LF@tmp%ld\n",nonTermCnt);
			printf("ADD LF@tmp%ld LF@tmp%ld LF@tmp%ld\n",nonTermCnt,currItem->id,currItem3->id);

			freeItem(currItem);
			freeItem(currItemOperand);
			freeItem(currItem3);
			//indent
			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			eStackPushNonTerm(stack);
			stack->head->id = nonTermCnt;
			return SUCCESS;
			break;
		case RULE2: //E->(E)
			//)
			currItem = eStackPopItem(stack);	
			freeItem(currItem);
			//E
			currItem = eStackPopItem(stack);	
			freeItem(currItem);
			//()
			currItem = eStackPopItem(stack);	
			freeItem(currItem);
			//indent
			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			eStackPushNonTerm(stack);
			stack->head->id = nonTermCnt;
			// nonTermCnt++;
			return SUCCESS;
			break;
		case RULE3: //E->i
			nonTermCnt++;
			//2,$var,"test",2.1, true,false,null,
			if(currItem->token->type == TOK_INT_LIT){
				printf("defvar LF@tmp%ld\n",nonTermCnt);
				printf("move LF@tmp%ld int@%d\n",nonTermCnt, currItem->token->attribute.intVal);
			}
			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			//indent
			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			eStackPushNonTerm(stack);
			stack->head->id = nonTermCnt;
			return SUCCESS;
			break;
		case RULE_ERROR:
			return ERR_SYN_ANALYSIS;
			break;
	}
	return ERR_SYN_ANALYSIS;
}


eRules_t exprFindRule(eStack_t *stack){
	int currState = E_STATE_START;
	bool repeat = true;
	int tokenType;
	eItem_t *currItem;

	while(repeat){
		switch(currState){

			//starting state
			case E_STATE_START:
				currItem = stack->head;
				//if term is at the top of stack, 
				//then it can only be rule E->i or E->(E)
				if(currItem->type == TERM){	 //todo: handle if null
					tokenType = tokenTypeToeType(currItem->token);
					if(tokenType == P_ERROR){
						currState = RULESTATES_ERROR;
						break;
					}
					//found (, expecting rule E->(E)
					if(tokenType ==  P_RIGHT_PAREN){
						currState = RULE2_EXPECTED1;
						break;
					}
					//found term, expecting rule E->i
					else if(tokenType == P_ID){
						currState = RULE3_EXPECTED1;
						break;
					}
					else{
						currState = RULESTATES_ERROR;
					}
				}
				//if not, then it is one of these rules: E->E+E, E->E*E,...
				if(currItem->type == NONTERM){
					//E -> E operand E, for instance: E -> E*E
					currState = RULE1_EXPECTED1;
				}
				else{
					currState = RULESTATES_ERROR;
				}
				break;
				//we fount nonterm -> E on top of stack

			/* 
			 * rule: E->E+E
			 * now we expect that the next token will be operant
			 */
			case RULE1_EXPECTED1:
				currItem = currItem->next;
				tokenType = tokenTypeToeType(currItem->token);
				if(tokenType == P_ERROR){
					currState = RULESTATES_ERROR;
					break;
				}
				if(tokenType != P_ID || tokenType != P_LEFT_PAREN || tokenType != P_RIGHT_PAREN){
					currState = RULE1_EXPECTED2;
				}
				else{
					currState = RULESTATES_ERROR;
				}
				break;
			/* 
			 * rule: E->E+E
			 * now we expect that the next token will be NONTERM E
			 */
			case RULE1_EXPECTED2:
				currItem = currItem->next;
				if(currItem->type == NONTERM){
					currState = RULE1_EXPECTED3;
				}
				else{
					currState = RULESTATES_ERROR;
				}
				break;
			/* 
			 * rule: E->E+E
			 * now we expect that the next token will be INDENT '<'
			 */
			case RULE1_EXPECTED3:
				currItem = currItem->next;
				if(currItem->type == INDENT){
					repeat = false;
					return RULE1;
				}
				else{
					currState = RULESTATES_ERROR;
				}
				break;

			//rule E -> (E)
			//expecting: E
			case RULE2_EXPECTED1:
				currItem = currItem->next;
				if(currItem->type == NONTERM){
					currState = RULE2_EXPECTED2;
				}
				else{
					currState = RULESTATES_ERROR;
				}
				break;
			//rule E -> (E)
			//expecting: )
			case RULE2_EXPECTED2:
				currItem = currItem->next;
				tokenType = tokenTypeToeType(currItem->token);
				if(tokenType == P_ERROR){
					currState = RULESTATES_ERROR;
					break;
				}
				if(tokenType ==  P_LEFT_PAREN){
					currState = RULE2_EXPECTED3;
				}
				else{
					currState = RULESTATES_ERROR;
				}
				break;
			//rule E->(E)
			//expecting: indent
			case RULE2_EXPECTED3:
				currItem = currItem->next;
				//next item needs to be indent
				if(currItem->type == INDENT){
					repeat = false;
					return RULE2;
				}
				else{
					currState = RULESTATES_ERROR;
				}
				break;

			//rule E -> i
			//expecting: indent
			case RULE3_EXPECTED1:
				currItem = currItem->next;
				//next item needs to be indent
				if(currItem->type == INDENT){//todo else error aj u ostatnych
					repeat = false;
					return RULE3;
				}
				else{
					currState = RULESTATES_ERROR;
				}
				break;
			case RULESTATES_ERROR:
				fprintf(stderr,"ERROR: wrong type of token in expression5");
				return RULE_ERROR;
				break;
			default: 
				fprintf(stderr,"ERROR: wrong type of token in expression6");
				return RULE_ERROR;


		}
	}
	return RULE_ERROR;
}

precTokenType_t tokenTypeToeType(token_t *token){
	tokenType_t type = token->type;
	if(type == TOK_KEYWORD){ //if type of token is keyword, we check keywords
		keyword_t keyword = token->attribute.kwVal;
		switch(keyword){
			case KW_TRUE:
			case KW_FALSE:
			case KW_NULL:
				return P_ID;
			default:
				fprintf(stderr,"ERROR: wrong type of token in expression7");
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
		case TOK_LEFT_PAREN:
			return P_LEFT_PAREN;
		case TOK_RIGHT_PAREN:
			return P_RIGHT_PAREN;
		case TOK_SEMICOLON:
			return P_SEMICOLON;
		case TOK_STRING_LIT:            
    	case TOK_INT_LIT:             
    	case TOK_DEC_LIT:      
		case TOK_VARIABLE:
			return P_ID;
		default:
			fprintf(stderr,"ERROR: wrong type of token in expression 8");
			return P_ERROR;
	}
}

//function used for debugging, function prints type of term
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
		case TOK_LEFT_PAREN:
			return "(";
		case TOK_RIGHT_PAREN:
			return ")";
		case TOK_SEMICOLON: //semicolon behaves like DOLLAR, so end of expression
			return ";";
		case TOK_STRING_LIT:            
    	case TOK_INT_LIT:             
    	case TOK_DEC_LIT: 
		case TOK_VARIABLE:               
			return "i";
		default:
			return "<tokenTypeToStr error>";
    }
}