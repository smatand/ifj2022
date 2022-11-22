
#include <stdlib.h>
#include <stdio.h>

#include "generator.h"
#include "dll.h"
#include "error.h"

void genInit() {
    printf(".IFJcode22\n");

    printf("defvar GF@tmp1\n");
    printf("defvar GF@tmp2\n");
    printf("defvar GF@tmp3\n");
    printf("defvar GF@tmp4\n");
    printf("defvar GF@tmp5\n");
    printf("defvar GF@trash\n");

    printf("createframe\n");
}

/**************** STANDARD LIBRARY (PHP) ****************/
void gen_reads() {
    printf("##### reads() function #####\n");


}

void gen_readi() {
    printf("##### readi() function #####\n");


}

void gen_readf() {
    printf("##### readf() function #####\n");


}

void gen_write() {
    printf("##### write() function #####\n");


}

void gen_floatval() {
    printf("##### floatval() function #####\n");


}

void gen_intval() {
    printf("##### intval() function #####\n");


}

void gen_strval() {
    printf("##### strval() function #####\n");


}

void gen_strlen(/*string*/) {
    printf("##### strlen(string $s) function #####\n");


}

void gen_substring(/*string, int, int*/) {
    printf("##### substring(string $s, int $i, int $j) function #####\n");


}