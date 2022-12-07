/**
 * Project: Translator of language IFJ22
 * @file expr.c
 * @author Tomáš Frátrik - xfratr01
 * @author Andrej Smatana - xsmata03
 * @author Martin Maršalek - xmarsa15
 * 
 * @brief Implementation of precedence analyzer unit
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "expr_stack.h"
#include "expr.h"
#include "scanner.h"
#include "str.h"
#include "error.h"
#include "generator.h"
#include "parser.h"




// int main(){
// 	// token_t *token2 = tokenInit();
// 	// scanToken(token2);
// 	int returnToken;
// 	token_t *token = tokenInit();
// 	scanToken(token);
// 	token_t *token2 = tokenInit();
// 	scanToken(token2);
// 	Parser_t *Parser = NULL;
// 	int returnVal = exprParse(token,token2,&returnToken,Parser);
// 	// token = tokenInit();
// 	// scanToken(token);
// 	// returnVal = exprParse(token,NULL,&returnToken);
// 	(void)returnToken;
// 	(void)returnVal;
// 	if(returnVal != SUCCESS)
// 	{
// 		printf("ERROR: %d\n",returnVal);
// 	}
// 	else
// 	{
// 		puts("SUCCESS");
// 	}
// }

int exprParse(token_t *firstToken, token_t *secondToken, int *returnToken, Parser_t *parser)
{
	
	int returnVal = SUCCESS;
	size_t nonTermCnt = 0;
	// bool generateCode = true;
	// <: shift with indent
	// >: reduce
	// =: shift without indent
	// !: error
	const char precedenceTable[TABLE_SIZE][TABLE_SIZE] = {
		//*    +    -    /    .    <    >    >=   <=   ===  !==  (    )    i    $
		{'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // *
		{'<', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // +
		{'<', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // -
		{'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // /
		{'<', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // .
		{'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'}, // <
		{'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'}, // >
		{'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'}, // >=
		{'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'}, // <=
		{'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'}, // ===
		{'<', '<', '<', '<', '<', '!', '!', '!', '!', '!', '!', '<', '>', '<', '>'}, // !==
		{'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', '!'}, // (
		{'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '!', '>', '!', '>'}, // )
		{'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '!', '>', '!', '>'}, // i
		{'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '!', '<', '!'}, // $
	};
	if (firstToken == NULL)
	{
		fprintf(stderr, "expr,ERROR: internal Error \n");
		return ERR_SYN_ANALYSIS;
	}
	
	bool secondTokenDelay = false;
	bool generateCode = true;

	if (secondToken != NULL)
	{
		generateCode = false;
		secondTokenDelay = true; // expression isn't assigned to anything
	}
	generateCode = true;
	if (generateCode)
	{
		printf("\n####################\n");
		printf("#### Expression ####\n");
		printf("####################\n");
		printf("createframe\n");
		printf("pushframe\n");
	}

	bool continueParsing = true;
	bool scanAnotherToken = true;

	/* Init stack */
	eStack_t estack;
	eStack_t *stack = &estack;
	eStackInit(stack);
	eStackPushDollar(stack);

	// initialize
	int stackTokenType;
	int incomingTokenType;
	char operation;
	(void)returnToken;
	(void)parser;
	token_t *incomingToken = copyToken(parser->currentToken);
	eItem_t *closestTerm = NULL;
	eItem_t *incomingTokenItem;
	// stackPrint(stack);

	while (continueParsing)
	{

		if (scanAnotherToken)
		{
			incomingTokenItem = eItemInit(incomingToken, TERM);
			if (incomingTokenItem == NULL)
			{
				freeItem(incomingTokenItem);
				eStackEmptyAll(stack);
				return ERR_INTERNAL;
			}
		}
		incomingTokenType = tokenTypeToeType(incomingToken);
		if (incomingTokenType == P_ERROR)
		{
			freeItem(incomingTokenItem);
			eStackEmptyAll(stack);
			return ERR_INTERNAL;
		}
		closestTerm = findClosestTerm(stack); // closest term in stack

		// if closest term is end of the stack
		if (closestTerm->type == DOLLAR)
		{
			stackTokenType = P_DOLLAR;
		}
		else
		{
			stackTokenType = tokenTypeToeType(closestTerm->token);
			if (stackTokenType == P_ERROR)
			{
				freeItem(incomingTokenItem);
				eStackEmptyAll(stack);
				return ERR_INTERNAL;
			}
		}
		scanAnotherToken = true;

		if (incomingTokenType == P_SEMICOLON)
		{
			operation = '!';
		}
		else
		{
			// find operation in precedence table
			operation = precedenceTable[stackTokenType][incomingTokenType];
		}
		switch (operation)
		{
		case '<': // shift with indent
			returnVal = eStackPushIndent(stack);
			if (returnVal != SUCCESS)
			{
				freeItem(incomingTokenItem);
				eStackEmptyAll(stack);
				return returnVal;
			}
			eStackPushItem(stack, incomingTokenItem);
			if (returnVal != SUCCESS)
			{
				freeItem(incomingTokenItem);
				eStackEmptyAll(stack);
				return returnVal;
			}
			break;
		case '>': // reduce
			scanAnotherToken = false;
			returnVal = exprReduce(stack, &nonTermCnt, generateCode, parser);
			if (returnVal != SUCCESS)
			{
				freeItem(incomingTokenItem);
				eStackEmptyAll(stack);
				return returnVal;
			}
			break;
		case '=': // shift without pushing indent
			eStackPushItem(stack, incomingTokenItem);
			break;
		case '!': // error or end of expression
			/**
			 * this case still can be correct, if it is right parenthesis ')'
			 * because this could be case where we encounter right closing
			 * bracket of its statement [example: if(expression')' ],
			 * therefore we send this ending token to top bottom for further analysis
			 * or if it is end of line, finished by semicolon ';'.
			 */
			if (incomingTokenType == P_RIGHT_PAREN || incomingTokenType == P_SEMICOLON)
			{
				// printf("JE TO SEMICOLOMN\n");
				// we need to end expression with $E in stack
				while (stack->head->next->type != DOLLAR)
				{
					returnVal = exprReduce(stack, &nonTermCnt, generateCode, parser);
					if (returnVal != SUCCESS)
					{
						freeItem(incomingTokenItem);
						eStackEmptyAll(stack);
						return returnVal;
					}
					// stackPrint(stack);
				}
				continueParsing = false;
				// free last token, ; OR ) and empty whole stack
				freeItem(incomingTokenItem);
				eStackEmptyAll(stack);
				*returnToken = TOK_RIGHT_PAREN;
				(incomingTokenType == P_RIGHT_PAREN)?(*returnToken = TOK_RIGHT_PAREN):(*returnToken = TOK_SEMICOLON);
				exprGenerateEndingCode(nonTermCnt,generateCode,incomingTokenType);
				return returnVal;
			}

			else
			{
				freeItem(incomingTokenItem);
				eStackEmptyAll(stack);
				fprintf(stderr, "ERROR: wrong type of token in expression2");
				return ERR_SYN_ANALYSIS;
			}
			break;
		default:
			freeItem(incomingTokenItem);
			eStackEmptyAll(stack);
			fprintf(stderr, "ERROR: wrong type of token in expression3");
			return ERR_SYN_ANALYSIS;
		}

		// stackPrint(stack);
		if (scanAnotherToken)
		{
			// if (secondTokenDelay == false)
			// {
				// incomingToken = tokenInit();
				getNextToken(parser);
				incomingToken = copyToken(parser->currentToken);

			// }
			// this happens only if expression is not assigned to anything
			// this expression is processed if there are no errors, but its result
			// is thrown away
			// else
			// {
			// 	// getNextToken(parser);
			// 	incomingToken = copyToken(parser->nextToken);
			// 	secondTokenDelay = false;
			// }
		}
	}
	// printf("popframe\n");
	eStackEmptyAll(stack);
	return SUCCESS;
}

void exprGenerateEndingCode(size_t nonTermCnt,bool generateCode,int type){
	if (generateCode)
	{
		printf("pushs LF@tmp%ld\n",nonTermCnt);
		if (type == P_RIGHT_PAREN)
		{
			printf("call exprRightParen\n");
			
		}
		else
		{
			printf("call exprSemicolon\n");
		}
		printf("popframe\n");
		printf("\n");
	}
}

int generateCode_defvar(eItem_t *item, size_t *nonTermCnt, Parser_t *parser)
{
	int type = item->token->type;
	token_t *token = item->token;
	htab_pair_t *pair;
	printf("defvar LF@tmp%ld\n", *nonTermCnt);
	//generate definition code based on the type of token
	switch (type)
	{
	case TOK_INT_LIT:
		printf("move LF@tmp%ld int@%s\n", *nonTermCnt, convertIntToIFJ(token->attribute.intVal));
		return SUCCESS;
		break;
	case TOK_DEC_LIT:
		printf("move LF@tmp%ld float@%s\n", *nonTermCnt, convertFloatToIFJ(token->attribute.decVal));
		return SUCCESS;
		break;
	case TOK_STRING_LIT:
		printf("defvar LF@_tmp%ld\n", *nonTermCnt);
		printf("move LF@_tmp%ld string@%s\n", *nonTermCnt, convertStringToIFJ(token->attribute.strVal->str));
		printf("move LF@tmp%ld LF@_tmp%ld \n", *nonTermCnt, *nonTermCnt);
		return SUCCESS;
		break;
	case TOK_VARIABLE:
		//try to find variable in the table of symbols
		pair = htab_find(parser->localSymStack->top->table, token->attribute.strVal->str);
		if (pair == NULL)
		{
			// free
			fprintf(stderr,"PARSER ERROR (EXPRESSION) couldn't find variable: %s\n",token->attribute.strVal->str);
			return ERR_SEM_UNDEFINED_VAR;
		}
		printf("defvar LF@_tmp%ld\n", *nonTermCnt);
		printf("popframe\n");
		printf("pushs LF@%%%s%%\n", token->attribute.strVal->str);
		printf("pushframe\n");
		printf("pops LF@_tmp%ld \n", *nonTermCnt);
		printf("move LF@tmp%ld LF@_tmp%ld \n", *nonTermCnt, *nonTermCnt);
		return SUCCESS;
		break;
	case TOK_KEYWORD:
		if (token->attribute.kwVal == KW_NULL)
		{
			printf("move LF@tmp%ld nil@nil\n", *nonTermCnt);
		}
		break;
	default:
		return ERR_SYN_ANALYSIS;
		break;
	}
	return SUCCESS;
}

void generateCode_operation(eItem_t *item1, eItem_t *item2, eItem_t *operationItem, size_t *nonTermCnt)
{
	int operation = operationItem->token->type;
	printf("defvar LF@tmp%ld\n", *nonTermCnt);
	printf("pushs LF@tmp%ld\n", item2->id);
	printf("pushs LF@tmp%ld\n", item1->id);
	//generate code -> operation based on the operation token
	switch (operation)
	{
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
	case TOK_COMPARISON:
		printf("pushs string@compare\n");
		break;
	case TOK_NEG_COMPARISON:
		printf("pushs string@ncompare\n");
		break;
	case TOK_GREATER:
		printf("pushs string@greater\n");
		break;
	case TOK_LESS:
		printf("pushs string@lesser\n");
		break;
	case TOK_GREATER_EQUAL:
		printf("pushs string@greatereq\n");
		break;
	case TOK_LESS_EQUAL:
		printf("pushs string@lessereq\n");
		break;
	default:
		break;
	}
	printf("call $compute\n");
	printf("pops LF@tmp%ld\n", *nonTermCnt);
}

int exprReduce(eStack_t *stack, size_t *nonTermCnt, bool generateCode, Parser_t *parser)
{
	int ruleType = exprFindRule(stack);
	eItem_t *currItem = stack->head;
	switch (ruleType)
	{
	case RULE1: // E->E+E, E->E<E, ...
		(*nonTermCnt)++;
		currItem = eStackPopItem(stack);
		eItem_t *currItemOperand = eStackPopItem(stack);
		eItem_t *currItem2 = eStackPopItem(stack);

		if(generateCode)
		{
			generateCode_operation(currItem, currItem2, currItemOperand, nonTermCnt);
		}

		freeItem(currItem);
		freeItem(currItemOperand);
		freeItem(currItem2);
		// free indent
		currItem = eStackPopItem(stack);
		freeItem(currItem);

		eStackPushNonTerm(stack);
		stack->head->id = *nonTermCnt;
		return SUCCESS;
		break;
	case RULE2: // E->(E)
		// free )
		currItem = eStackPopItem(stack);
		freeItem(currItem);
		// free E
		currItem = eStackPopItem(stack);
		freeItem(currItem);
		// free (
		currItem = eStackPopItem(stack);
		freeItem(currItem);
		// free indent
		currItem = eStackPopItem(stack);
		freeItem(currItem);

		eStackPushNonTerm(stack);
		stack->head->id = *nonTermCnt;
		return SUCCESS;
		break;
	case RULE3: // E->i
		(*nonTermCnt)++;
		int ret = SUCCESS;
		if (generateCode)
		{
			ret = generateCode_defvar(currItem, nonTermCnt, parser);
		}

		// free E
		currItem = eStackPopItem(stack);
		freeItem(currItem);

		// free indent
		currItem = eStackPopItem(stack);
		freeItem(currItem);
		eStackPushNonTerm(stack);
		stack->head->id = *nonTermCnt;
		return ret;
		break;

	case RULE_ERROR:
		return ERR_SYN_ANALYSIS;
		break;
	}
	return ERR_SYN_ANALYSIS;
}

eRules_t exprFindRule(eStack_t *stack)
{
	int currState = E_STATE_START;
	bool repeat = true;
	int tokenType;
	eItem_t *currItem;

	while (repeat)
	{
		switch (currState)
		{

		// starting state
		case E_STATE_START:
			currItem = stack->head;
			// if term is at the top of stack,
			// then it can only be rule E->i or E->(E)
			if (currItem->type == TERM)
			{ // todo: handle if null
				tokenType = tokenTypeToeType(currItem->token);
				if (tokenType == P_ERROR)
				{
					currState = RULESTATES_ERROR;
					break;
				}
				// found (, expecting rule E->(E)
				if (tokenType == P_RIGHT_PAREN)
				{
					currState = RULE2_EXPECTED1;
					break;
				}
				// found term, expecting rule E->i
				else if (tokenType == P_ID)
				{
					currState = RULE3_EXPECTED1;
					break;
				}
				else
				{
					currState = RULESTATES_ERROR;
				}
			}
			// if not, then it is one of these rules: E->E+E, E->E*E,...
			if (currItem->type == NONTERM)
			{
				// E -> E operand E, for instance: E -> E*E
				currState = RULE1_EXPECTED1;
			}
			else
			{
				currState = RULESTATES_ERROR;
			}
			break;
			// we fount nonterm -> E on top of stack

		/*
		 * rule: E->E+E
		 * now we expect that the next token will be operant
		 */
		case RULE1_EXPECTED1:
			currItem = currItem->next;
			tokenType = tokenTypeToeType(currItem->token);
			if (tokenType == P_ERROR)
			{
				currState = RULESTATES_ERROR;
				break;
			}
			if (tokenType != P_ID || tokenType != P_LEFT_PAREN || tokenType != P_RIGHT_PAREN)
			{
				currState = RULE1_EXPECTED2;
			}
			else
			{
				currState = RULESTATES_ERROR;
			}
			break;
		/*
		 * rule: E->E+E
		 * now we expect that the next token will be NONTERM E
		 */
		case RULE1_EXPECTED2:
			currItem = currItem->next;
			if (currItem->type == NONTERM)
			{
				currState = RULE1_EXPECTED3;
			}
			else
			{
				currState = RULESTATES_ERROR;
			}
			break;
		/*
		 * rule: E->E+E
		 * now we expect that the next token will be INDENT '<'
		 */
		case RULE1_EXPECTED3:
			currItem = currItem->next;
			if (currItem->type == INDENT)
			{
				repeat = false;
				return RULE1;
			}
			else
			{
				currState = RULESTATES_ERROR;
			}
			break;

		// rule E -> (E)
		// expecting: E
		case RULE2_EXPECTED1:
			currItem = currItem->next;
			if (currItem->type == NONTERM)
			{
				currState = RULE2_EXPECTED2;
			}
			else
			{
				currState = RULESTATES_ERROR;
			}
			break;
		// rule E -> (E)
		// expecting: )
		case RULE2_EXPECTED2:
			currItem = currItem->next;
			tokenType = tokenTypeToeType(currItem->token);
			if (tokenType == P_ERROR)
			{
				currState = RULESTATES_ERROR;
				break;
			}
			if (tokenType == P_LEFT_PAREN)
			{
				currState = RULE2_EXPECTED3;
			}
			else
			{
				currState = RULESTATES_ERROR;
			}
			break;
		// rule E->(E)
		// expecting: indent
		case RULE2_EXPECTED3:
			currItem = currItem->next;
			// next item needs to be indent
			if (currItem->type == INDENT)
			{
				repeat = false;
				return RULE2;
			}
			else
			{
				currState = RULESTATES_ERROR;
			}
			break;

		// rule E -> i
		// expecting: indent
		case RULE3_EXPECTED1:
			currItem = currItem->next;
			// next item needs to be indent
			if (currItem->type == INDENT)
			{ // todo else error aj u ostatnych
				repeat = false;
				return RULE3;
			}
			else
			{
				currState = RULESTATES_ERROR;
			}
			break;
		case RULESTATES_ERROR:
			fprintf(stderr, "ERROR: wrong type of token in expression5");
			return RULE_ERROR;
			break;
		default:
			fprintf(stderr, "ERROR: wrong type of token in expression6");
			return RULE_ERROR;
		}
	}
	return RULE_ERROR;
}

eItem_t *findClosestTerm(eStack_t *stack)
{
	eItem_t *currItem = stack->head;
	while (currItem->type == INDENT || currItem->type == NONTERM)
	{
		currItem = currItem->next;
	}
	return currItem;
}

precTokenType_t tokenTypeToeType(token_t *token)
{
	tokenType_t type = token->type;
	if (type == TOK_KEYWORD)
	{ // if type of token is keyword, we check keywords
		keyword_t keyword = token->attribute.kwVal;
		switch (keyword)
		{
		case KW_TRUE:
		case KW_FALSE:
		case KW_NULL:
			return P_ID;
		default:
			fprintf(stderr, "ERROR: wrong type of token in expression7");
			return P_ERROR;
		}
	}
	switch (type)
	{
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
		fprintf(stderr, "ERROR: wrong type of token in expression 8\n");
		fprintf(stderr, "token = %d\n",type);
		return P_ERROR;
	}
}

char *tokenTypeToStr(token_t *token)
{
	tokenType_t type = token->type;
	if (type == TOK_KEYWORD)
	{
		keyword_t keyword = token->attribute.kwVal;
		switch (keyword)
		{
		case KW_TRUE:
		case KW_FALSE:
		case KW_NULL:
			return "i";
		default:
			return "error";
		}
	}
	switch (type)
	{
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
	case TOK_SEMICOLON: // semicolon behaves like DOLLAR, so end of expression
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
