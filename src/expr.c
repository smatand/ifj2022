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



int main(){ 
	token_t *returnToken = tokenInit();
	token_t *token = tokenInit();
	token_t *secondToken = tokenInit();
	scanToken(token);
	scanToken(secondToken);
	exprParse(token,secondToken,returnToken);

}

int exprParse(token_t *firstToken,token_t *secondToken, token_t *returnToken){
	const char precedenceTable[TABLE_SIZE][TABLE_SIZE] = {
	//  *    +    -    /    .    <    >    >=   <=   ===  !==  (    )    i    $ 
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

	// int returnCode = SUCCESS;
	bool secondTokenDelay = false;
	if(secondToken != NULL){
		secondTokenDelay = true;
	}

	bool continueParsing = true;	
	bool scanAnotherToken = true;
	//init stack
	eStack_t estack;
	eStack_t *stack = &estack; 
	eStackInit(stack);
	eStackPushDollar(stack);

	//
	int stackTokenType;
	int incomingTokenType;
	char operation;
	(void) returnToken;
	returnToken = NULL;
	token_t *incomingToken = firstToken;
	eItem_t *closestTerm = NULL;
	stackPrint(stack);

	while(continueParsing){
		eItem_t *incomingTokenItem = eItemInit(incomingToken,TERM);
		incomingTokenType = tokenTypeToeType(incomingToken);
		closestTerm = findClosestTerm(stack); //closest term in stack
		if(incomingToken->type == TOK_EOF){
			exit(ERR_SYN_ANALYSIS);
		}
		if(closestTerm->type == DOLLAR){ //if it is the end of stack
			stackTokenType = P_DOLLAR;
		}
		else{
			stackTokenType = tokenTypeToeType(closestTerm->token);
		}
		scanAnotherToken = true;


		if(incomingTokenType == P_SEMICOLON){
			operation = '!';
		}
		else{
        	operation = precedenceTable[stackTokenType][incomingTokenType];
		}
		
		switch(operation){
					case '<': //shift with indent
						exprShift(stack,incomingTokenItem);
						break;
					case '>': //reduce
						// (closestTerm->type == DOLLAR) ? printf("Najblizsi term: DOLLAR") : printf("Najblizsi term: %s\n",tokenTypeToStr(closestTerm->token));
						exprReduce(stack);
						break;
					case '=': //shift without pushing indent
						eStackPushItem(stack,incomingTokenItem); 
						break;
					case '!': //we found error
						/**
						 * error still can be correct, if it is right parenthesis ')'
						 * because this could be case where we encounter right closing 
						 * bracket of its statement (like if(expressin) ),
						 * therefore we send this to top bottom for further analysis
						 */
						if(incomingTokenType == P_RIGHT_PAREN || incomingTokenType == P_SEMICOLON){
							//we need to end expression with $E
							while(stack->head->next->type != DOLLAR){ 
								exprReduce(stack);
								stackPrint(stack);
							}
							continueParsing = false;
							// staci return )
							//free stack a tak
							returnToken = incomingToken;
							return 0;
							break;
						}
						else{
							exit(ERR_SYN_ANALYSIS);
						}
						break;
					default:
						exit(ERR_SYN_ANALYSIS);
				}
		if(incomingTokenItem->type == DOLLAR || !continueParsing){
            freeItem(incomingTokenItem);
            break;
        }
		//reduction = we loop thru while again with same token
        if(operation == '>'){
            scanAnotherToken = false;
        }
        stackPrint(stack); //

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
	eStackEmptyAll(stack);
	return 0;

}

void exprShift(eStack_t *stack, eItem_t *item){
    eStackPushIndent(stack);
    eItem_t *newItem = eItemInit(item->token,TERM);
    eStackPushItem(stack,newItem);
}


eItem_t *findClosestTerm(eStack_t *stack){
    eItem_t *currItem = stack->head;
    while(currItem->type == INDENT || currItem->type == NONTERM){
        currItem = currItem->next;
    }
    return currItem;
}

void exprReduce(eStack_t *stack){
	int ruleType = exprFindRule(stack);
	eItem_t *currItem = stack->head;
	switch(ruleType){
		case RULE1://4 times repeating, is just for now, waiting for code generation
			currItem = eStackPopItem(stack);	
			free(currItem);
			currItem = eStackPopItem(stack);	
			free(currItem);
			currItem = eStackPopItem(stack);	
			free(currItem);
			currItem = eStackPopItem(stack);	
			free(currItem);
			eStackPushNonTerm(stack);
			break;
		case RULE2:
			currItem = eStackPopItem(stack);	
			free(currItem);
			currItem = eStackPopItem(stack);	
			free(currItem);
			currItem = eStackPopItem(stack);	
			free(currItem);
			currItem = eStackPopItem(stack);	
			free(currItem);
			eStackPushNonTerm(stack);
			break;
		case RULE3:
			currItem = eStackPopItem(stack);	
			free(currItem);
			currItem = eStackPopItem(stack);	
			free(currItem);
			eStackPushNonTerm(stack);
			break;
		case RULE_ERROR:
			exit(ERR_SYN_ANALYSIS);
			// printf("rule error");
			break;
	}
}


eRules_t exprFindRule(eStack_t *stack){
	// stackPrint(stack);
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
					if(tokenType ==  P_RIGHT_PAREN){
						currState = RULE2_EXPECTED1;
						break;
					}
					else if(tokenType == P_ID){
						currState = RULE3_EXPECTED1;
					}
					else{
						currState = RULESTATES_ERROR;
					}
				}
				//if not then it is rule 1 = E->E+E,...
				if(currItem->type == NONTERM){
					//E -> E operand E, for instance: E -> E*E
					currState = RULE1_EXPECTED1;
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
			case RULE2_EXPECTED2:
				currItem = currItem->next;
				tokenType = tokenTypeToeType(currItem->token);
				if(tokenType ==  P_LEFT_PAREN){
					currState = RULE2_EXPECTED3;
				}
				else{
					currState = RULESTATES_ERROR;
				}
				break;
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
				exit(ERR_SYN_ANALYSIS);
			default: 
				exit(ERR_SYN_ANALYSIS);


		}
	}
	return RULE_ERROR;
}
//todo co patri pod id  co moze byt vo vyraze?
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
				exit(ERR_SYN_ANALYSIS);
				// return P_ERROR;
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
		case TOK_IDENTIFIER:
			return P_ID;
		default:
			exit(ERR_SYN_ANALYSIS);
			// return P_ERROR;
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
		case TOK_LEFT_PAREN:
			return "(";
		case TOK_RIGHT_PAREN:
			return ")";
		case TOK_SEMICOLON: //semicolon behaves like DOLLAR, so end of expression
			return ";";
		case TOK_STRING_LIT:            
    	case TOK_INT_LIT:             
    	case TOK_DEC_LIT: 
		case TOK_IDENTIFIER:               
			return "i";
		default:
			return "<tokenTypeToStr error>";
    }
}