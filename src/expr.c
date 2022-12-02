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
#include"./generator.h"

int main(){ 
	token_t *token = tokenInit();
	scanToken(token);
	int returnToken;
	int returnVal = exprParse(token,NULL,&returnToken);
	(void)returnToken;
	(void)returnVal;


}


void gen_checkType(){
	printf(
			"\n"
			"#checktype(var1,var2,operand)\n"
			"label checktype\n"
			"createframe\n"
			"pushframe\n"
			"defvar LF@_var1\n"
			"defvar LF@_var2\n"
			"defvar LF@_operand\n"
			"defvar LF@type_var1\n"
			"defvar LF@type_var2 #pravy operand\n"
			"pops LF@_operand\n"
			"pops LF@_var2\n"
			"pops LF@_var1\n"
			"type LF@type_var1 LF@_var1\n"
			"type LF@type_var2 LF@_var2\n"
			"jumpifeq arit1 LF@_operand string@add\n"
			"jumpifeq arit1 LF@_operand string@sub\n"
			"jumpifeq arit1 LF@_operand string@mul\n"
			"jumpifeq arit2 LF@_operand string@div\n"
			"jumpifeq concat LF@_operand string@concat\n"
			"jumpifeq rel1 LF@_operand string@greater\n"


			"label concat\n"
			"jumpifneq skip_concat1 LF@type_var1 string@nil\n"
			"move LF@_var1 string@\n"
			"move LF@type_var1 string@string\n"
			"label skip_concat1\n"
			"jumpifneq skip_concat2 LF@type_var2 string@nil\n"
			"move LF@_var2 string@\n"
			"move LF@type_var2 string@string\n"
			"label skip_concat2\n"
			"jumpifneq error_sem7 LF@type_var1 string@string\n"
			"jumpifneq error_sem7 LF@type_var2 string@string\n"
			"jump checkEnd\n"
			//valid operands in +;-;*;/ -> int dec null



			"#arit1 = + *\n"
			"label arit1\n"//what is the type of the first operant ?
			"defvar LF@booltest\n"
			"move LF@booltest bool@true\n"
			"jumpifeq arit1_firstint LF@type_var1 string@int\n"
			"jumpifeq arit1_firstfloat LF@type_var1 string@float\n"
			"jumpifeq arit1_firstnull LF@type_var1 string@nil\n"
			"jump error_sem7 #wrong type of operand\n"
			
			"label arit1_firstint\n"
			"jumpifeq checkEnd LF@type_var2 string@int\n"
			"jumpifneq arit1_skip1 LF@type_var2 string@float\n"
			"pushs LF@_var1\n"
			"call floatval\n"
			"pops LF@_var1\n"
			"jump checkEnd\n"
			"label arit1_skip1\n"
			"jumpifneq error_sem7 LF@type_var2 string@nil\n"
			"move LF@_var2 int@0\n"
			"jump checkEnd\n"
			
			"label arit1_firstfloat\n"
			"jumpifeq checkEnd LF@type_var2 string@float\n"
			"jumpifneq arit1_skip4 LF@type_var2 string@int\n"
			"pushs LF@_var2\n"
			"call floatval\n"
			"pops LF@_var2\n"
			"label arit1_skip4\n"
			"jump checkEnd\n"

			"label arit1_firstnull\n"
			"jumpifneq arit1_skip7 LF@type_var2 string@nil\n"
			"move LF@_var1 int@0\n"
			"move LF@_var2 int@0\n"
			"jump checkEnd\n"
			"label arit1_skip7\n"
			"jumpifneq arit1_skip8 LF@type_var2 string@int\n"
			"move LF@_var1 int@0\n"
			"label arit1_skip8\n"
			"jump checkEnd\n"


			//arit2 -> divide
			"label arit2\n"
			"jumpifeq arit2_seconod LF@type_var1 string@float\n"
			"jumpifneq arit2_skip1 LF@type_var1 string@int\n"
			"pushs LF@_var1\n"
			"call floatval\n"
			"pops LF@_var1\n"
			"jump arit2_second\n"
			"label arit2_skip1\n"
			"jumpifneq error_sem7 LF@type_var1 string@nil\n"
			"move LF@_var1 float@0x0.0p+0 \n"
			"jump arit2_second\n"

			"label arit2_second\n"
			"jumpifeq checkEnd LF@type_var2 string@float\n"
			"jumpifneq arit2_skip2 LF@type_var2 string@int\n"
			"pushs LF@_var2\n"
			"call floatval\n"
			"pops LF@_var2\n"
			"jump checkEnd\n"
			"label arit2_skip2\n"
			"jumpifneq error_sem7 LF@type_var2 string@nil\n"
			"move LF@_var2 float@0x0.0p+0\n"
			"jump checkEnd\n"

			// "label rel1\n"
			// "jumpifeq rel1_skipnull LF@type_var1 string@nil\n"
			// "jumpifeq rel1_skipnull LF@type_var2 string@nil\n"


			// "label rel1_skipnull\n"
			// "move LF@_var1 bool@false\n"
			// "move LF@_var2 bool@false\n"
			// "jump checkEnd\n"
			
			"label error_sem7\n"
			"write string@checkType_error_sem_7\n"
			"exit int@7\n"


			"label checkEnd\n"
			"pushs LF@_var2\n"
			"pushs LF@_var1\n"
			"pushs LF@_operand\n"
			"popframe\n"
			"return\n"
			"\n"
			);
}

void gen_compute(){
	printf(
			"\n"
			"##############################\n"
			"##compute(op1,op2,operation)##\n"
			"##############################\n"
			"label compute\n"
			"createframe\n"
			"pushframe\n"
			"defvar LF@_returnVal\n"
			"defvar LF@_op1\n"
			"defvar LF@_op2\n"
			"defvar LF@_operation\n"
			"call checktype\n"
			"pops LF@_operation\n"
			"pops LF@_op1\n"
			"pops LF@_op2\n"
			
			"jumpifeq computeAdd LF@_operation string@add\n"				
			"jumpifeq computeMul LF@_operation string@mul\n"				
			"jumpifeq computeSub LF@_operation string@sub\n"				
			"jumpifeq computeDiv LF@_operation string@div\n"				
			"jumpifeq computeConcat LF@_operation string@concat\n"				
			"jumpifeq computeGreater LF@_operation string@greater\n"				

			"label computeAdd\n"
			"add LF@_returnVal LF@_op1 LF@_op2\n"
			"jump computeEnd\n"

			"label computeMul\n"
			"mul LF@_returnVal LF@_op1 LF@_op2\n"
			"jump computeEnd\n"

			"label computeSub\n"
			"sub LF@_returnVal LF@_op1 LF@_op2\n"
			"jump computeEnd\n"

			"label computeDiv\n"
			"div LF@_returnVal LF@_op1 LF@_op2\n"
			"jump computeEnd\n"

			"label computeConcat\n"
			"concat LF@_returnVal LF@_op1 LF@_op2\n"
			"jump computeEnd\n"

			"label computeGreater\n"
			"gt LF@_returnVal LF@_op1 LF@_op2\n"
			"jump computeEnd\n"


			"label computeEnd\n"
			"pushs LF@_returnVal\n"
		  	"popframe\n"
			"return\n"
			"\n"
			);
}


int exprParse(token_t *firstToken, token_t *secondToken, int *returnToken){
	printf(".IFJcode22\n");

	puts("call $skipOperations");
	gen_checkType();
	gen_compute();
	gen_builtin_functions();
	puts("label $skipOperations");
	printf(
			"createframe\n"
			"pushframe\n"
			"defvar LF@$var\n"
			"move LF@$var int@3\n"
			);
	printf("\n####################\n");
	printf("#### Expression ####\n");
	printf("####################\n");
	printf("createframe\n");
	printf("pushframe\n");

	int returnVal = SUCCESS;
	size_t nonTermCnt = 0;
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
						returnVal=exprReduce(stack, &nonTermCnt);
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
								returnVal=exprReduce(stack,&nonTermCnt);
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
							printf("defvar LF@type_tmp%ld\n",nonTermCnt);
							// printf("defvar LF@exprTrue%ld\n",nonTermCnt);
							// printf("defvar LF@exprFalse%ld\n",nonTermCnt);
							// puts("move LF@exprTrue bool@true");
							// printf("type LF@type_tmp%ld LF@tmp%ld\n",nonTermCnt,nonTermCnt);
							// printf("jumpifeq exprBoolret LF@type_tmp%ld LF@exprTrue\n",nonTermCnt);
							// printf("jumpifeq exprBoolret LF@type_tmp%ld string@false\n",nonTermCnt);
							printf("write LF@tmp%ld\n",nonTermCnt);
							puts("jump exprEnd");
							// printf("label exprBoolret\n");
							// printf("defvar LF@retBool\n");
							//printf("eq LF@tmp%ld LF@tmp%ld bool@true");
							// printf("jumpifeq exprTrueret LF@_tmp%ld bool@true\n",nonTermCnt);
							// printf("write string@false\n");
							// puts("jump exprEnd");
							// printf("label exprTrueret\n");
							// printf("write string@true\n");
							// puts("jump exprEnd");
							printf("label exprEnd\n");
							printf("popframe\n");
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


void generateCode_defvar(eItem_t *item, size_t *nonTermCnt){
	int type = item->token->type;
	token_t *token = item->token;
	printf("defvar LF@tmp%ld\n",*nonTermCnt);
	switch(type){
		case TOK_INT_LIT:
			printf("move LF@tmp%ld int@%s\n",*nonTermCnt, convertIntToIFJ(token->attribute.intVal));
			break;
		case TOK_DEC_LIT:
			printf("move LF@tmp%ld float@%s\n",*nonTermCnt, convertFloatToIFJ(token->attribute.decVal));
			break;
		case TOK_STRING_LIT:
			printf("defvar LF@_tmp%ld\n",*nonTermCnt);
			printf("move LF@_tmp%ld string@%s\n",*nonTermCnt, convertStringToIFJ(token->attribute.strVal->str));
			printf("move LF@tmp%ld LF@_tmp%ld \n",*nonTermCnt,*nonTermCnt);
			break;
		case TOK_VARIABLE:
			printf("defvar LF@_tmp%ld\n",*nonTermCnt);
			printf("popframe\n");
			printf("pushs LF@%s\n",token->attribute.strVal->str);
			printf("pushframe\n");
			printf("pops LF@_tmp%ld \n",*nonTermCnt);
			printf("move LF@tmp%ld LF@_tmp%ld \n",*nonTermCnt,*nonTermCnt);
			break;
		case TOK_KEYWORD:
			if(token->attribute.kwVal == KW_NULL){
				printf("move LF@tmp%ld nil@nil\n",*nonTermCnt);
			}
			break;
		default:
			break;			
	}
}

eItem_t *findClosestTerm(eStack_t *stack){
    eItem_t *currItem = stack->head;
    while(currItem->type == INDENT || currItem->type == NONTERM){
        currItem = currItem->next;
    }
    return currItem;
}

void generateCode_operation(eItem_t *item1,eItem_t *item2, eItem_t *operationItem,size_t *nonTermCnt){
	// token_t *token1 = item1->token;
	// token_t *token2 = item2->token;
	// token_t *tokenOperation = operation->token;
	//A+B
	//item1 = A
	//item2 = B
	int operation = operationItem->token->type;
	printf("defvar LF@tmp%ld\n",*nonTermCnt);
	printf("pushs LF@tmp%ld\n",item2->id);
	printf("pushs LF@tmp%ld\n",item1->id);
	switch(operation){
		case TOK_PLUS:
			printf("pushs string@add\n");
			break;
		case TOK_MINUS:
			printf("pushs string@sub\n");		
			break;
		case TOK_STAR:
			printf("pushs string@mul\n");
			break;
		case TOK_SLASH:
			printf("pushs string@div\n");
			break;
		case TOK_DOT:
			printf("pushs string@concat\n");
			break;
		default:
			break;
	}
	printf("call compute\n");
	printf("pops LF@tmp%ld\n",*nonTermCnt);

}

int exprReduce(eStack_t *stack,size_t *nonTermCnt){
	int ruleType = exprFindRule(stack);
	eItem_t *currItem = stack->head;
	switch(ruleType){
		case RULE1: //E->E+E, E->E<E, ...
			(*nonTermCnt)++;
			currItem = eStackPopItem(stack);	
			eItem_t *currItemOperand = eStackPopItem(stack);	
			eItem_t *currItem2 = eStackPopItem(stack);	

			generateCode_operation(currItem,currItem2,currItemOperand,nonTermCnt);

			freeItem(currItem);
			freeItem(currItemOperand);
			freeItem(currItem2);
			//indent
			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			eStackPushNonTerm(stack);
			stack->head->id = *nonTermCnt;
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
			stack->head->id = *nonTermCnt;
			return SUCCESS;
			break;
		case RULE3: //E->i
			(*nonTermCnt)++;
			generateCode_defvar(currItem,nonTermCnt);
			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			//indent
			currItem = eStackPopItem(stack);	
			freeItem(currItem);

			eStackPushNonTerm(stack);
			stack->head->id = *nonTermCnt;
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