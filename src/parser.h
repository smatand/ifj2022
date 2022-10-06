/**
 * @file parser.h
 *
 * @brief Implementation of parser module for IFJ22
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>

/**********************************************************************************   LL GRAMMAR   *****************************
<program>  ->  <prolog> <units> <program_end>
<prolog>  ->  "<?php" "declare(strict_types=1);"
<units>  ->  <unit> <units>
<units>  ->  ε
<unit>  ->  <function_definition>
<unit>  ->  <statements>

<function_definition> ->  "function" ID "(" <params> ")" ":" <type> "{" EOL <statements> EOL "}" EOL
<function_definition> ->  ε
<params>  ->  <param> <param_n>
<params>  ->  ε
<param>  ->  <type> "$" ID
<param_n>  ->  "," <type> "$" ID
<param_n>  ->  ε

<type>  ->  INT
<type>  ->  STRING
<type>  ->  FLOAT

<statements>  ->  <statement> <statements>
<statements>  ->  ε

<statement>  ->  "$" ID "=" <assignment> ";"     // assignment
<statement>  ->  "if" "(" <expression> ")" "{" <statements> "}" "else" "{" <statements> "}"     // conditional
<statement>  ->  "while" "(" <expression> ")" "{" <statements> "}"     // loop
<statement>  ->  ID "(" <arguments> ")" ";"     // function call sans assignment
<statement>  ->  "return" <return_value> ";"     // return statement

<assignment>  ->  <expression>
<assignment>  ->  ID "(" <arguments> ")"     // assignment via function

<arguments>  ->  <term> <argument_n>
<arguments>  ->  ε
<argument_n>  ->  "," <value>
<argument_n>  ->  ε

<return_value>  ->  <expression>
<return_value>  ->  ε

<term>  ->  "$" ID
<term>  ->  INT_LITERAL
<term>  ->  STRING_LITERAL
<term>  ->  FLOAT_LITERAL
<term>  ->  NULL

<program_end>  ->  "?>" EOF
<program_end>  ->  EOF
**********************************************************************************   LL GRAMMAR   *****************************/

int parse_source();

int r_program();

int r_prolog();

int r_units();

int r_program_end();

#endif /* PARSER_H */