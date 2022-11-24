#include <stdlib.h>
#include <stdio.h>

#include "generator.h"
#include "dll.h"
#include "error.h"
#include "str.h"

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

void gen_substring() {
    printf(
        "label substring\n"
        "pushframe\n"

        "defvar LF@_string\n"
        "defvar LF@_index_from\n"
        "defvar LF@_index_to\n"
        "defvar LF@_retval\n"

        "move LF@_retval string@\n"

        "defvar LF@_tmp\n"
        "defvar LF@_strlen\n"

        "pops LF@_index_to # $j\n"
        "pops LF@_index_from # $i\n"
        "pops LF@_string # $s\n"

        "# errors\n"
        "lt LF@_tmp LF@_index_from int@0 # $i < 0\n"
        "jumpifeq substring_return_null LF@_tmp bool@true\n"
        "lt LF@_tmp LF@_index_to int@0 # $j < 0\n"
        "jumpifeq substring_return_null LF@_tmp bool@true\n"
        "gt LF@_tmp LF@_index_from LF@_index_to # $i > $j\n"
        "jumpifeq substring_return_null LF@_tmp bool@true\n"

        "strlen LF@_strlen LF@_string\n"
        "gt LF@_tmp LF@_index_from LF@_strlen # $i > strlen($s)\n"
        "jumpifeq substring_return_null LF@_tmp bool@true\n"
        "jumpifeq substring_return_null LF@_index_from LF@_strlen\n"
        "gt LF@_tmp LF@_index_to LF@_strlen # $j > strlen($s)\n"
        "jumpifeq substring_return_null LF@_tmp bool@true\n"
        "jumpifeq substring_return_null LF@_index_to LF@_index_from\n"


        "# getchar <var> <symb1> <symb2>\n"
        "label substring_while\n"
        "getchar LF@_tmp LF@_string LF@_index_from\n"

        "concat LF@_retval LF@_retval LF@_tmp\n"

        "add LF@_index_from LF@_index_from int@1 # $i++\n"
        "jumpifneq substring_while LF@_index_from LF@_index_to\n"


        "jump substring_end\n"

        "label substring_return_null\n"
        "move LF@_retval nil@nil\n"

        "label substring_end\n"
        "pushs LF@_retval\n"
        "popframe\n"
        "return\n"
    );
}

void gen_ord() {
    printf(
        "label ord\n"
        "pushframe\n"

        "defvar LF@_string\n"
        "defvar LF@_retval\n"
        "pops LF@_string\n"

        "jumpifeq ord_empty_string LF@_string string@\n"

        "stri2int LF@_retval LF@_string int@0\n"
        "jump ord_end\n"


        "label ord_empty_string\n"
        "move LF@_retval int@0\n"

        "label ord_end\n"
        "pushs LF@_retval\n"
        "popframe\n"
        "return\n"
        );
}

void gen_chr() {
    printf(
        "label chr\n"
        "pushframe\n"

        "defvar LF@_int\n"
        "defvar LF@_retval\n"
        "pops LF@_int\n"

        "int2char LF@_retval LF@_int\n"
        "jump chr_end\n"

        "label chr_end\n"
        "pushs LF@_retval\n"
        "popframe\n"
        "return\n"
        );
}

void gen_builtin_functions() {
    gen_reads();
    gen_readi();
    gen_readf();
    gen_write();
    gen_floatval();
    gen_intval();
    gen_strval();
    gen_substring();
    gen_ord();
    gen_chr();
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

char * convertStringToIFJ(char * str) {
    if (str == NULL) {
        return NULL;
    }

    char * ptr = str;
    int retVal;
    string_t * retStr = stringInit(&retVal);
    if (retVal == ERR_INTERNAL) {
        exit(ERR_INTERNAL);
    }

    if (*ptr == '\0') {
        return "";
    }

    while (*ptr != '\0') {
        if (*ptr == '\\') {
            charPushBack(retStr, '\\');
        } else if (*ptr == 35) { // #
            strPushBack(retStr, "\\035", 3);
        }
        else if (*ptr <= 32) {
            charPushBack(retStr, '\\');
            charPushBack(retStr, '0');
            charPushBack(retStr, (*ptr / 10) + 48);
            charPushBack(retStr, (*ptr % 10) + 48);
        } else {
            charPushBack(retStr, *ptr);
        }
        ptr++;
    }    

    charPushBack(retStr, '\0');
    return retStr->str;
}