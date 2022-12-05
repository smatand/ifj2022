#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "generator.h"
#include "scanner.h"
#include "dll.h"
#include "error.h"
#include "str.h"

/********************************************************/
/**************** STANDARD LIBRARY (PHP) ****************/
/********************************************************/

void gen_reads() {
    printf(
            "label $reads\n" 
            "createframe\n"
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
            "label $readi\n"
            "createframe\n"
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
            "label $readf\n"
            "createframe\n"
            "pushframe\n"

            "defvar LF@_retval\n"
            "read LF@_retval float\n" 
            "pushs LF@_retval\n"

            "popframe\n"
            "return\n"
            );
}
void gen_write() {
    printf("jump $write_end\n");
    printf("label $write\n");
    printf("createframe\n");
    printf("pushframe\n");
    printf("defvar LF@_count\n");
    printf("defvar LF@_toPrint\n");
    printf("defvar LF@_tmp\n");
    printf("pops LF@_count\n");
    printf("eq LF@_tmp LF@_count int@0\n");
    printf("jumpifeq $write_ret LF@_tmp bool@true\n");
    printf("label $write_loop\n");
    printf("pops LF@_toPrint\n");
    printf("write LF@_toPrint\n");
    printf("sub LF@_count LF@_count int@1\n");
    printf("jumpifneq $write_loop LF@_count int@0\n");

    printf("label $write_ret\n");
    printf("popframe\n");
    printf("return\n");
    printf("label $write_end\n");
}

void gen_floatval() {
    printf(
            "label $floatval\n"
            "createframe\n"
            "pushframe\n"
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
        "label $intval\n"
        "createframe\n"
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
    printf(
        "label $strval\n"
        "createframe\n"
        "pushframe\n"

        "defvar LF@_retval\n"
        "defvar LF@_condition\n"

        "pops LF@_retval\n"

        "eq LF@_condition LF@_retval nil@nil\n"
        "jumpifeq strval_end LF@_condition bool@false\n" // it's not a nil value

        "move LF@_retval string@\n"

        "label strval_end\n"
        "pushs LF@_retval\n"
        "popframe\n"
        "return\n"
    );
}

void gen_strlen(/*string*/) {
}

void gen_substring() {
    printf(
        "label $substring\n"
        "createframe\n"
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
        "label $ord\n"
        "createframe\n"
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
        "label $chr\n"
        "createframe\n"
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
    gen_write();
    gen_reads();
    gen_readi();
    gen_readf();
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

void gen_checkType(){
	printf(
			"\n"
			"#checktype(var1,var2,operand)\n"
			"label $checktype\n"
			"createframe\n"
			"pushframe\n"
			"defvar LF@_bool\n"
			"defvar LF@_rel2eq\n"
			"defvar LF@_rel2ans\n"
			"defvar LF@_jumprel2\n"
			"defvar LF@_jumprel3\n"
			"move LF@_jumprel2 bool@false\n"
			"move LF@_jumprel3 bool@false\n"

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
			"jumpifeq compare LF@_operand string@compare\n"
			"jumpifeq compare LF@_operand string@ncompare\n"
			"jumpifeq rel1 LF@_operand string@greater\n"
			"jumpifeq rel1 LF@_operand string@lesser\n"
			"jumpifeq rel2 LF@_operand string@greatereq\n"
			"jumpifeq rel3 LF@_operand string@lessereq\n"

			"label rel3\n"
			"move LF@_jumprel3 bool@true\n"
			"jump rel2\n"
			"label rel3_cmp\n"
			"eq LF@_rel2eq LF@_var1 LF@_var2\n"
			"not LF@_bool LF@_bool\n"
			"or LF@_rel2ans LF@_bool LF@_rel2eq\n"
			"jumpifeq true_label LF@_rel2ans bool@true\n"
			"jump false_label\n"

			//>=
			"label rel2\n"
			"move LF@_jumprel2 bool@true\n"
			"jumpifeq true_label LF@type_var1 string@nil\n"
			"jumpifeq true_label LF@type_var2 string@nil\n"
			"jump rel2_jmp1\n"
			"label rel2_cmp\n"
			"jumpifeq rel3_cmp LF@_jumprel3 bool@true\n"
			"eq LF@_rel2eq LF@_var1 LF@_var2\n"
			"or LF@_rel2ans LF@_bool LF@_rel2eq\n"
			"jumpifeq true_label LF@_rel2ans bool@true\n"
			"jump false_label\n"

			// < 
			"label rel1\n"
			"jumpifeq false_label LF@type_var1 string@nil\n"
			"jumpifeq false_label LF@type_var2 string@nil\n"
			"label rel2_jmp1\n"
			"jumpifeq rel1_firststring LF@type_var1 string@string\n"
			"jumpifeq rel1_firstint LF@type_var1 string@int\n"
			"jumpifeq rel1_firstfloat LF@type_var1 string@float\n"

			"label rel1_firstint\n"
			"jumpifeq rel1_secondfloat LF@type_var2 string@float\n"
			"gt LF@_bool LF@_var1 LF@_var2\n"
			"jumpifeq rel2_cmp LF@_jumprel2 bool@true\n #if it is also >= or <="
			"gt LF@_bool LF@_var1 LF@_var2\n"
			"move LF@_var1 LF@_bool\n"
			"move LF@_var2 LF@_bool\n"
			"jump checkEnd\n"
			"label rel1_secondfloat\n"
			"pushs LF@_var1\n"
			"call $floatval\n"
			"pops LF@_var1\n"
			"gt LF@_bool LF@_var1 LF@_var2\n"
			"jumpifeq rel2_cmp LF@_jumprel2 bool@true\n #if it is also >= or <="
			"gt LF@_bool LF@_var1 LF@_var2\n"
			"move LF@_var1 LF@_bool\n"
			"move LF@_var2 LF@_bool\n"
			"jump checkEnd\n"
			"label rel1_firstfloat\n"
			"pushs LF@_var2\n"
			"call $floatval\n"
			"pops LF@_var2\n"
			"gt LF@_bool LF@_var1 LF@_var2\n"
			"jumpifeq rel2_cmp LF@_jumprel2 bool@true\n #if it is also >= or <="
			"gt LF@_bool LF@_var1 LF@_var2\n"
			"move LF@_var1 LF@_bool\n"
			"move LF@_var2 LF@_bool\n"
			"jump checkEnd\n"
			"label rel1_firststring\n"
			"jumpifneq error_sem7 LF@type_var2 string@string\n"
			//is shows corret bool, only when they are swapped 
			"gt LF@_bool LF@_var2 LF@_var1\n"
			"jumpifeq rel2_cmp LF@_jumprel2 bool@true\n #if it is also >= or <="
			"gt LF@_bool LF@_var2 LF@_var1\n"
			"move LF@_var1 LF@_bool\n"
			"move LF@_var2 LF@_bool\n"
			"jump checkEnd\n"

			"label false_label\n"
			"move LF@_var1 bool@false\n"
			"move LF@_var2 bool@false\n"
			"jump checkEnd\n"
			"label true_label\n"
			"move LF@_var1 bool@true\n"
			"move LF@_var2 bool@true\n"
			"jump checkEnd\n"

			"label compare\n"
			"jumpifeq skip_compare1 LF@type_var1 LF@type_var2\n"
			"jump false_label\n"
			"label skip_compare1\n"
			"jumpifneq skip_compare2 LF@_var1 LF@_var2\n"
			"jump true_label\n"
			"label skip_compare2\n"
			"jump false_label\n"
			"jump checkEnd\n"

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


	);
	printf(
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
			"call $floatval\n"
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
			"call $floatval\n"
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
			"call $floatval\n"
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
			"call $floatval\n"
			"pops LF@_var2\n"
			"jump checkEnd\n"
			"label arit2_skip2\n"
			"jumpifneq error_sem7 LF@type_var2 string@nil\n"
			"move LF@_var2 float@0x0.0p+0\n"
			"jump checkEnd\n"
			
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
			"label $compute\n"
			"createframe\n"
			"pushframe\n"
			// "defvar LF@_returnNull\n"
			"defvar LF@_returnVal\n"
			"defvar LF@_op1\n"
			"defvar LF@_op2\n"
			"defvar LF@_operation\n"
			"call $checktype\n"
			"pops LF@_operation\n"
			"pops LF@_op1\n"
			"pops LF@_op2\n"
			
			"jumpifeq computeAdd LF@_operation string@add\n"				
			"jumpifeq computeMul LF@_operation string@mul\n"				
			"jumpifeq computeSub LF@_operation string@sub\n"				
			"jumpifeq computeDiv LF@_operation string@div\n"				
			"jumpifeq computeConcat LF@_operation string@concat\n"				
			"jumpifeq computeCompare LF@_operation string@compare\n"				
			"jumpifeq computeNCompare LF@_operation string@ncompare\n"				
			"jumpifeq computeGreater LF@_operation string@greater\n"				
			"jumpifeq computeLesser LF@_operation string@lesser\n"				
			"jumpifeq computeGreatereq LF@_operation string@greatereq\n"				
			"jumpifeq computeLessereq LF@_operation string@lessereq\n"				

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

			"label computeCompare\n"
			"move LF@_returnVal LF@_op1\n"
			"jump computeEnd\n"


			//reversing return from compare
			"label computeNCompare\n"
			"jumpifeq ncmp_jmp LF@_op1 bool@true\n"
			"move LF@_returnVal bool@true\n"
			"jump computeEnd\n"
			"label ncmp_jmp\n"
			"move LF@_returnVal bool@false\n"
			"jump computeEnd\n"

			"label computeGreater\n"
			"move LF@_returnVal LF@_op1\n"
			"jump computeEnd\n"

			//reversing return from greater
			"label computeLesser\n"
			"jumpifeq lesser_jmp LF@_op1 bool@true\n"
			"move LF@_returnVal bool@true\n"
			"jump computeEnd\n"
			"label lesser_jmp\n"
			"move LF@_returnVal bool@false\n"
			"jump computeEnd\n"


			"label computeGreatereq\n"
			"move LF@_returnVal LF@_op1\n"
			"jump computeEnd\n"

			"label computeLessereq\n"
			"move LF@_returnVal LF@_op1\n"
			"jump computeEnd\n"

			"label computeEnd\n"
			"pushs LF@_returnVal\n"
		  	"popframe\n"
			"return\n"
			"\n"
			);
}

void genInit() {
    printf(".IFJcode22\n");

    printf("defvar GF@typeCheck\n");
    printf("defvar GF@tmp2\n");
    printf("defvar GF@tmp3\n");
    printf("defvar GF@tmp4\n");

    printf("createframe\n");
    printf("pushframe\n");
    printf("jump _START\n");

    gen_checkType();
	gen_compute();
    gen_builtin_functions();

    printf("\nlabel _START\n");
    printf("createframe\n");

    // todo call to main function using dll algorithm (?)
}

void genEnd()
{
    printf("jump _END\n");
    printf("label _TYPE_SEM_ERR\n");
    printf("exit int@4\n");
    printf("label _NIL_SEM_ERR\n");
    printf("exit int@8\n");
    printf("label _END\n"); // the last line
}

char * convertStringToIFJ(char * str) {
    if (str == NULL) {
        return NULL;
    }

    char * ptr = str;
    int retVal = 0;

    // just to use the library of str.h
    string_t * tmp = stringInit(&retVal);
    if (retVal) {
        exit(retVal);
    }

    if (*ptr == '\0') {
        return "";
    }

    while (*ptr != '\0') {
        if (*ptr == 92) { // backslash
            strPushBack(tmp, "\\092", 4);
        } else if (*ptr == 35) { // #
            strPushBack(tmp, "\\035", 4);
        }
        else if (*ptr <= 32) {
            charPushBack(tmp, '\\');
            charPushBack(tmp, '0');
            charPushBack(tmp, (*ptr / 10) + 48);
            charPushBack(tmp, (*ptr % 10) + 48);
        } else {
            charPushBack(tmp, *ptr);
        }
        ptr++;
    }    

    charPushBack(tmp, '\0');
    
    // reuse the defined variable to return the demanded string
    ptr = malloc(tmp->realLen);
    if (ptr == NULL) {
        exit(ERR_INTERNAL);
    }

    memcpy(ptr, tmp->str, tmp->realLen);
    stringDestroy(tmp);
    
    return ptr;
}

char * convertIntToIFJ(int x) {
    char * ptr = malloc(12);
    if (ptr == NULL) {
        exit(ERR_INTERNAL);
    }

    snprintf(ptr, 12, "%d", x);

    return ptr;
}

char * convertFloatToIFJ(float x) {
    char * ptr = malloc(32);
    if (ptr == NULL) {
        exit(ERR_INTERNAL);
    }

    snprintf(ptr, 32, "%a", x); // printing in hexadecimal format as required

    return ptr;
}

void genFunctionLabel(char * functionName) {
    printf(
        "\njump $%s_end\n"
        "label $%s\n"
        "pushframe\n"
        "defvar LF@%%retval\n"
        "move LF@%%retval nil@nil\n"
        "defvar LF@%%_countArgs\n"
        "pops LF@%%_countArgs\n"
    , functionName, functionName);
}

void genFunctionEnd(char * functionName) {
    printf(
        "label $%s_ret\n"
        "pushs LF@%%retval\n"
        // todo check for retval with function type (exit 7 (type compatibility))
        "popframe\n"
        "return\n"
        "label $%s_end\n\n"
    , functionName, functionName);
}

//int genToPush(char * toPush) {
//    CONCAT_STRINGS_DLL(")
//    
//}

void genFunctionParam(char * functionName, char * paramName) {
    static int paramCounter = 0;

    printf(
        "defvar LF@%s\n"
        "move LF@%s LF@%s\n"

        "defvar LF@%s\n"
        "move LF@%s LF@%s\n");
}

void genFunctionParamType(keyword_t kw, int count) {
    printf("defvar LF@_paramType%%%d\n", count);

    switch (kw) {
        case KW_INT:
            printf("move LF@_paramType%%%d string@int\n", count);
            break;
        case KW_FLOAT:
            printf("move LF@_paramType%%%d string@float\n", count);
            break;
        case KW_STRING:
            printf("move LF@_paramType%%%d string@string\n", count);
            break;
        default: // todo
            fprintf(stderr, "Error: line %d", __LINE__);
            break;
    }
}

void genFunctionRetType(keyword_t kw) {
    printf("defvar LF@_returnType\n");

    switch (kw) {
        case KW_INT:
            printf("move LF@_returnType string@int\n");
            break;
        case KW_FLOAT:
            printf("move LF@_returnType string@float\n");
            break;
        case KW_STRING:
            printf("move LF@_returnType string@string\n");
            break;
        case KW_VOID:
            printf("move LF@_returnType string@void\n");
            break;
        default: // todo
            fprintf(stderr, "Error: line %d", __LINE__);
            break;
    }
}

void genTypeCheck(int count) {
    printf("type GF@typeCheck LF@param%d\n", count);
    printf("pushs GF@typeCheck\n");
    printf("pushs LF@_paramType%%%d\n", count);
    printf("jumpifneqs _TYPE_SEM_ERR\n");
}

void genFunctionAmountOfParamsCheck(int count) {
	printf("defvar LF@%%_count_def_par # TOTO JE NOVE\n"); // TODO remove comment
	printf("move LF@%%_count_def_par int@%d\n", count);
	printf("pushs LF@%%_count_def_par\n");
	printf("pushs LF@%%_countArgs\n");
	printf("jumpifneqs _TYPE_SEM_ERR\n");
}
    //switch(token->type) {
    //    case TOK_STRING_LIT: ;
    //        char * ptr = convertStringToIFJ(token->attribute.strVal->str); // dynamically allocated str
    //        //CONCAT_STRINGS_DLL("write string@", ptr);
    //        printf("write string@%s", ptr);
    //        free(ptr); 
    //        break;
    //    case TOK_INT_LIT: ;
    //        char * ptr2 = convertIntToIFJ(token->attribute.intVal);
    //        //CONCAT_STRINGS_DLL("write int@", ptr2);
    //        printf("write int@%s", ptr2);
    //        free(ptr2);
    //        break;
    //    case TOK_DEC_LIT: ;
    //        char * ptr3 = convertFloatToIFJ(token->attribute.decVal);
    //        //CONCAT_STRINGS_DLL("write float@", ptr3);
    //        printf("write float@%s", ptr3);
    //        free(ptr3);
    //        break;
    //    case TOK_KEYWORD:
    //        if (token->attribute.kwVal == KW_NULL) {
    //            //DLLInsertLast(list, "write nil@nil", strlen("write nil@nil")+1);
    //            printf("write nil@nil");
    //        } else if (token->attribute.kwVal == KW_TRUE) {
    //            //DLLInsertLast(list, "write bool@true", strlen("write bool@true")+1);
    //            printf("write bool@true");
    //        }
    //        break;
    //    default:
    //        return ERR_SEM_PARAMS;
    //}
