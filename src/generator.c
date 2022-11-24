
#include <stdlib.h>
#include <stdio.h>

#include "generator.h"
#include "dll.h"
#include "error.h"

/********************************************************/
/**************** STANDARD LIBRARY (PHP) ****************/
/********************************************************/

void gen_reads() {
    printf(
            "label reads\n"
            "pushframe\n" // save frame

            "defvar LF@_retval\n" // local variable
            "read TF@_retval string \n"
            "pushs TF@_retval\n"

            "popframe\n" // restore
            "return\n"
            );
}

void gen_readi() {
    printf(
            "label readi\n"
            "pushframe\n"

            "defvar LF@_retval\n"
            "read LF@_retval int\n" 
            "pushs LF@_retval\n"

            "popframe\n"
            "return\n"
            );
}

void gen_readf() {
    printf(
            "label readf\n"
            "pushframe\n"

            "defvar LF@_retval\n"
            "read LF@_retval float\n" 
            "pushs LF@_retval\n"

            "popframe\n"
            "return\n"
            );
}

void gen_write() {

}

void gen_floatval() {
    printf(
            "defvar LF@_param # also retval\n"
            "defvar LF@_type\n"
            "defvar LF@_condition\n"
            "pops LF@_param\n"
            
            "move LF@_condition bool@false\n"
            "type LF@_type LF@_param # int, bool, float, string or nil\n"
            
            "jumpifeq floatval_end LF@_type string@float # no need to convert\n"
            
            "jumpifeq int_to_float LF@_type string@int\n"
            
            "jumpifeq bool_to_float LF@_type string@bool\n"

            // todo: strnum, strbool rozsirenia?
            "move LF@_param float@0x0.0p+0\n" // nil to 0.0
            "jump floatval_end\n"

            "label bool_to_float\n"
            "eq LF@_condition LF@_param bool@true\n"
            "jumpifneq bool_false_to_float LF@_condition bool@true\n"
            
            "move LF@_param float@0x1.0p+0\n"
            "jump floatval_end\n"
            
            "label bool_false_to_float\n"
            "move LF@_param float@0x0.0p+0\n"
            "jump floatval_end\n"

            "label int_to_float\n"
            "int2float LF@_param LF@_param\n"

            "label floatval_end\n"
            "pushs LF@_param\n"
            "popframe\n"
            "return\n"
            );
}

void gen_intval() {
    printf(
        "label intval\n"
        "pushframe\n"

        "defvar LF@_param # also retval\n"
        "defvar LF@_type\n"
        "defvar LF@_condition\n"
        "pops LF@_param\n"

        "move LF@_condition bool@false\n"

        "type LF@_type LF@_param # int, bool, float, string or nil\n"

        "jumpifeq intval_end LF@_type string@int # no need to convert\n"

        "jumpifeq float_to_int LF@_type string@float\n"

        "jumpifeq bool_to_int LF@_type string@bool\n"

        "move LF@_param int@0\n"
        "jump intval_end \n"

        "label bool_to_int\n"
        "eq LF@_condition LF@_param bool@true\n"
        "jumpifneq bool_false_to_int LF@_condition bool@true\n"

        "move LF@_param int@1\n"
        "jump intval_end\n"

        "label bool_false_to_int\n"
        "move LF@_param int@0\n"
        "jump intval_end\n"

        "label float_to_int\n"
        "float2int LF@_param LF@_param\n"

        "label intval_end\n"
        "pushs LF@_param\n"
        "popframe\n"
        "return\n"
    );
}

void gen_strval() {
    // rozsirenie strnum not needed todo
}

void gen_strlen(/*string*/) {
}

void gen_substring(/*string, int, int*/) {
}

void gen_builtin_functions() {
    gen_reads();
    gen_readi();
    gen_readf();
    gen_write();
    gen_floatval();
    gen_intval();

    gen_strval();

    gen_strlen();
    gen_substring();
}
/***************************************************************/
/**************** END OF STANDARD LIBRARY (PHP) ****************/
/**************************************************************/

void genInit() {
    printf(".IFJcode22\n");

    printf("defvar GF@tmp1\n");
    printf("defvar GF@tmp2\n");
    printf("defvar GF@tmp3\n");
    printf("defvar GF@tmp4\n");


    printf("createframe\n");
    printf("call $main\n");

    gen_builtin_functions();

    printf("label $main\n");
}
