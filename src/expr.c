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
	token_t *token = tokenInit();
	scanToken(token);
	int returnVal = exprParse(token,NULL);
	(void)returnVal;


}

int exprParse(token_t *firstToken,token_t *secondToken){
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
		exit(ERR_INTERNAL);
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
	stackPrint(stack);

	while(continueParsing){

		if(scanAnotherToken){
			incomingTokenItem = eItemInit(incomingToken,TERM);
		}
		incomingTokenType = tokenTypeToeType(incomingToken);
		closestTerm = findClosestTerm(stack); //closest term in stack

		//if closest term is end of the stack
		if(closestTerm->type == DOLLAR){ 
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
			//find operation in precedence table
        	operation = precedenceTable[stackTokenType][incomingTokenType];
		}
		switch(operation){
					case '<': //shift with indent
						eStackPushIndent(stack);
						eStackPushItem(stack,incomingTokenItem);
						break;
					case '>': //reduce
						scanAnotherToken = false;
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
						 * or if it is end of line, finished by semicolon ';'.
						 */
						if(incomingTokenType == P_RIGHT_PAREN || incomingTokenType == P_SEMICOLON){
							//we need to end expression with $E in stack
							while(stack->head->next->type != DOLLAR){ 
								exprReduce(stack);
								stackPrint(stack);
							}
							continueParsing = false;
							freeItem(incomingTokenItem);
							eStackEmptyAll(stack);
							int retval;
							(incomingTokenType == P_RIGHT_PAREN) ? (retval =  TOK_RIGHT_PAREN): (retval =  TOK_SEMICOLON);
							return retval; 
						}
						else{
							eStackEmptyAll(stack);
							fprintf(stderr,"ERROR: wrong type of token in expression2");
							exit(ERR_SYN_ANALYSIS);
						}
						break;
					default:
						fprintf(stderr,"ERROR: wrong type of token in expression3");
						exit(ERR_SYN_ANALYSIS);
				}

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
		case RULE1: //E->E+E, E->E<E, ...
			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			//indent
			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			eStackPushNonTerm(stack);
			break;
		case RULE2: //E->(E)
			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			eStackPushNonTerm(stack);
			break;
		case RULE3: //E->i
			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			eStackPushNonTerm(stack);
			break;
		case RULE_ERROR:
			fprintf(stderr,"ERROR: wrong type of token in expression4");
			exit(ERR_SYN_ANALYSIS);
			break;
	}
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
				exit(ERR_SYN_ANALYSIS);
				break;
			default: 
				fprintf(stderr,"ERROR: wrong type of token in expression6");
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
				fprintf(stderr,"ERROR: wrong type of token in expression7");
				exit(ERR_SYN_ANALYSIS);
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
			exit(ERR_SYN_ANALYSIS);
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