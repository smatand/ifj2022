/**
 * @file expr.c 
 *
 * @brief Implementation of parser module for IFJ22
 */
#include<stdio.h>
#include"expr.h"


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





