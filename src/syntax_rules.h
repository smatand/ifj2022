/**
 * Project: Translator of language IFJ22
 * @file syntax_rules.h
 * @author János László Vasík - xvasik05
 * 
 * @brief Header file of syntax analysis unit
 */

#ifndef SYNTAX_RULES_H
#define SYNTAX_RULES_H

#include "parser.h"

// Rule #1 = <program>  ->  <prolog> <units> <program_end>
int rProgram(Parser_t *parser);

// Rule #2 = <prolog>  ->  "<?php declare(strict_types=1);"
int rProlog(Parser_t *parser);

/* Rule #3 = <units>  ->  <unit> <units>
 * Rule #4 = <units>  ->  ε
*/
int rUnits(Parser_t *parser);

/* Rule #5 = <unit>  ->  <function_definition>
 * Rule #6 = <unit>  ->  <statements>
*/
int rUnit(Parser_t *parser);

// Rule #7 = <function_definition> ->  "function" ID "(" <params> ")" ":" <type> "{" <statements> "}"
int rFunctionDefinition(Parser_t *parser);

/* Rule #8 = <params>  ->  <param> <params_n> 
 * Rule #9 = <params>  ->  ε
*/
int rParams(Parser_t *parser);

// Rule #10 = <param>  ->  <type> $ID
int rParam(Parser_t *parser);

// Rule #11 = <param_n>  ->  "," <param>
int rParam_n(Parser_t *parser);

/* Rule #12 = <params_n>  ->  <param_n> <params_n>
 * Rule #13 = <params_n>  ->  ε
*/
int rParams_n(Parser_t *parser);

/* Rule #14 = <type>  ->  INT
 * Rule #15 = <type>  ->  STRING
 * Rule #16 = <type>  ->  FLOAT
*/
int rType(Parser_t *parser);

/* Rule #17 = <statements>  ->  <variable_statement> <statements>
 * Rule #18 = <statements>  ->  <conditional_statement> <statements>
 * Rule #19 = <statements>  ->  <while_loop_statement> <statements>
 * Rule #20 = <statements>  ->  <function_call_statement> <statements>
 * Rule #21 = <statements>  ->  <return_statement> <statements>
 * Rule #22 = <statements>  ->  <expression> <statements>
 * Rule #23 = <statements>  ->  ε
*/
int rStatements(Parser_t *parser);

/* Rule #24 = <variable_statement>  -> <assignment_statement>
 * Rule #25 = <variable_statement>  -> <expression>
 * Rule #26 = <variable_statement>  -> $ID ";"
*/
int rVariableStatement(Parser_t *parser);

/* Rule #27 = <assignment_statement>  ->  $ID "=" <expression>
 * Rule #28 = <assignment_statement>  ->  $ID "=" <function_call_statement>
*/
int rAssignmentStatement(Parser_t *parser);

// Rule #29 = <conditional_statement>  ->  "if" "(" <expression> ")" "{" <statements> "}" "else" "{" <statements> "}"
int rConditionalStatement(Parser_t *parser);

// Rule #30 = <while_loop_statement>  ->  "while" "(" <expression> ")" "{" <statements> "}"
int rWhileLoopStatement(Parser_t *parser);

// Rule #31 = <function_call_statement>  ->  ID "(" <arguments> ")" ";"
int rFunctionCallStatement(Parser_t *parser);

// Rule #32 = <return_statement>  ->  "return" <return_value>
int rReturnStatement(Parser_t *parser);

/* Rule #33 = <arguments>  ->  <term> <arguments_n>
 * Rule #34 = <arguments>  ->  ε
*/ 
int rArguments(Parser_t *parser);

// Rule #35 = <argument_n>  ->  "," <term>
int rArgument_n(Parser_t *parser);

/* Rule #36 = <arguments_n>  ->  <argument_n> <arguments_n>
 * Rule #37 = <arguments_n>  ->  ε
*/
int rArguments_n(Parser_t *parser);

/* Rule #38 = <return_value>  ->  <expression>
 * Rule #39 = <return_value>  ->  ";"
*/
int rReturnValue(Parser_t *parser);

/* Rule #40 = <term>  ->  $ID
 * Rule #41 = <term>  ->  TOK_INT_LIT
 * Rule #42 = <term>  ->  TOK_STRING_LIT
 * Rule #43 = <term>  ->  TOK_DEC_LIT
 * Rule #44 = <term>  ->  KW_NULL
*/
int rTerm(Parser_t *parser);

/* Rule #45 = <program_end>  ->  "?>" EOF
 * Rule #46 = <program_end>  ->  EOF
*/
int rProgramEnd(Parser_t *parser);

#endif /* SYNTAX_RULES_H */