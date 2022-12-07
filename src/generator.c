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
    printf("label $reads\n"); 
    printf("createframe\n");
    printf("pushframe\n"); // save frame

    printf("defvar LF@_retval\n"); // local variable
    printf("read TF@_retval string\n");

    printf("pushs TF@_retvall\n");
    printf("popframe\n"); //restore
    printf("return\n");
}

void gen_readi() {
    printf("label $readi\n");
    printf("createframe\n");
    printf("pushframe\n");

    printf("defvar LF@_retval\n");
    printf("read LF@_retval int\n");

    printf("pushs LF@_retval\n");
    printf("popframe\n");
    printf("return\n");
}

void gen_readf() {
    printf("label $readf\n");
    printf("createframe\n");
    printf("pushframe\n");
    printf("defvar LF@_retval\n");
    printf("read LF@_retval float\n");

    printf("pushs LF@_retval\n");
    printf("popframe\n");
    printf("return\n");
}
void gen_write() {
    printf("label $write\n");
    printf("createframe\n");
    printf("pushframe\n");
    printf("defvar LF@_count\n");
    printf("defvar LF@_toPrint\n");
    printf("defvar LF@_tmp\n");
    printf("pops LF@_count\n");
    printf("eq LF@_tmp LF@_count int@0\n");
    printf("jumpifeq write_ret LF@_tmp bool@true\n");
    printf("label write_loop\n");
    printf("pops LF@_toPrint\n");
    printf("write LF@_toPrint\n");
    printf("sub LF@_count LF@_count int@1\n");
    printf("jumpifneq write_loop LF@_count int@0\n");

    printf("label write_ret\n");
    printf("popframe\n");
    printf("return\n");
}

void gen_floatval() {
    printf("label $floatval\n");
    printf("createframe\n");
    printf("pushframe\n");

    printf("defvar LF@_countArgs\n");
    printf("pops LF@_countArgs\n");
    printf("jumpifneq _TYPE_SEM_ERR LF@_countArgs int@1\n");

    printf("defvar LF@_param # also retval\n");
    printf("defvar LF@_type\n");
    printf("defvar LF@_condition\n");
    printf("pops LF@_param\n");
        
    printf("move LF@_condition bool@false\n");
    printf("type LF@_type LF@_param # int, bool, float, string or nil\n");
        
    printf("jumpifeq floatval_ret LF@_type string@float # no need to convert\n");
        
    printf("jumpifeq floatval_int_to_float LF@_type string@int\n");
        
    printf("jumpifeq floatval_bool_to_float LF@_type string@bool\n");

    printf("move LF@_param float@0x0.0p+0\n"); // nil to 0.0
    printf("jump floatval_ret\n");

    printf("label floatval_bool_to_float\n");
    printf("eq LF@_condition LF@_param bool@true\n");
    printf("jumpifneq floatval_bool_false_to_float LF@_condition bool@true\n");
        
    printf("move LF@_param float@0x1.0p+0\n");
    printf("jump floatval_ret\n");
        
    printf("label floatval_bool_false_to_float\n");
    printf("move LF@_param float@0x0.0p+0\n");
    printf("jump $floatval_ret\n");

    printf("label floatval_int_to_float\n");
    printf("int2float LF@_param LF@_param\n");

    printf("label floatval_ret\n");
    printf("pushs LF@_param\n");
    printf("popframe\n");
    printf("return\n");
}

void gen_intval() {
    printf("label $intval\n");
    printf("createframe\n");
    printf("pushframe\n");

    printf("defvar LF@_countArgs\n");
    printf("pops LF@_countArgs\n");
    printf("jumpifneq _TYPE_SEM_ERR LF@_countArgs int@1\n");

    printf("defvar LF@_param # also retval\n");
    printf("defvar LF@_type\n");
    printf("defvar LF@_condition\n");
    printf("pops LF@_param\n");

    printf("move LF@_condition bool@false\n");

    printf("type LF@_type LF@_param # int, bool, float, string or nil\n");

    printf("jumpifeq intval_end LF@_type string@int # no need to convert\n");

    printf("jumpifeq intval_float_to_int LF@_type string@float\n");

    printf("jumpifeq intval_bool_to_int LF@_type string@bool\n");

    printf("move LF@_param int@0\n");
    printf("jump intval_end \n");

    printf("label intval_bool_to_int\n");
    printf("eq LF@_condition LF@_param bool@true\n");
    printf("jumpifneq $floatval_bool_false_to_int LF@_condition bool@true\n");

    printf("move LF@_param int@1\n");
    printf("jump intval_end\n");

    printf("label floatval_bool_false_to_int\n");
    printf("move LF@_param int@0\n");
    printf("jump intval_end\n");

    printf("label intval_float_to_int\n");
    printf("float2int LF@_param LF@_param\n");

    printf("label intval_end\n");
    printf("pushs LF@_param\n");
    printf("popframe\n");
    printf("return\n");
}

void gen_strval() {
    printf("label $strval\n");
    printf("createframe\n");
    printf("pushframe\n");

    printf("defvar LF@_retval\n");
    printf("defvar LF@_condition\n");

    printf("defvar LF@_countArgs\n");
    printf("pops LF@_countArgs\n");
    printf("jumpifneq _TYPE_SEM_ERR LF@_countArgs int@1\n");


    printf("pops LF@_retval\n");
    printf("type GF@typeCheck LF@_retval\n");
    printf("jumpifeq _TYPE_SEM_ERR GF@typeCheck string@int\n"); // strnum
    printf("jumpifeq _TYPE_SEM_ERR GF@typeCheck string@float\n"); // strnum


    printf("jumpifeq strval_end GF@typeCheck string@string\n");

    printf("move LF@_retval string@\n"); // it is null then

    printf("label strval_end\n");
    printf("pushs LF@_retval\n");
    printf("popframe\n");
    printf("return\n");
}

void gen_strlen(/*string*/) {
    printf("label $strlen\n");
    printf("createframe\n");
    printf("pushframe\n");

    printf("defvar LF@_string\n");

    printf("defvar LF@_countArgs\n");
    printf("pops LF@_countArgs\n");
    printf("jumpifneq _TYPE_SEM_ERR LF@_countArgs int@1\n");

    printf("pops LF@_string\n");
    printf("type GF@typeCheck LF@_string\n");
    printf("jumpifneq _TYPE_SEM_ERR GF@typeCheck string@string\n");

    printf("defvar LF@_retval\n");
    printf("strlen LF@_retval LF@_string\n");

    printf("label strlen_end\n");
    printf("pushs LF@_retval\n");
    printf("popframe\n");
    printf("return\n");
}

void gen_substring() {
    printf("label $substring\n");
    printf("createframe\n");
    printf("pushframe\n");

    printf("defvar LF@_string\n");
    printf("defvar LF@_index_from\n");
    printf("defvar LF@_index_to\n");
    printf("defvar LF@_retval\n");

    printf("move LF@_retval string@\n");

    printf("defvar LF@_tmp\n");
    printf("defvar LF@_strlen\n");

    printf("defvar LF@_countArgs\n");
    printf("pops LF@_countArgs\n");
    printf("jumpifneq _TYPE_SEM_ERR LF@_countArgs int@3\n");


    printf("pops LF@_string # $s\n");
    printf("type GF@typeCheck LF@_string\n");
    printf("jumpifneq _TYPE_SEM_ERR GF@typeCheck string@string\n");

    printf("pops LF@_index_from # $i\n");
    printf("type GF@typeCheck LF@_index_from\n");
    printf("jumpifneq _TYPE_SEM_ERR GF@typeCheck string@int\n");

    printf("pops LF@_index_to # $j\n");
    printf("type GF@typeCheck LF@_index_to\n");
    printf("jumpifneq _TYPE_SEM_ERR GF@typeCheck string@int\n");

    printf("# errors\n");
    printf("lt LF@_tmp LF@_index_from int@0 # $i < 0\n");
    printf("jumpifeq substring_return_null LF@_tmp bool@true\n");
    printf("lt LF@_tmp LF@_index_to int@0 # $j < 0\n");
    printf("jumpifeq substring_return_null LF@_tmp bool@true\n");
    printf("gt LF@_tmp LF@_index_from LF@_index_to # $i > $j\n");
    printf("jumpifeq substring_return_null LF@_tmp bool@true\n");

    printf("strlen LF@_strlen LF@_string\n");
    printf("gt LF@_tmp LF@_index_from LF@_strlen # $i > strlen($s)\n");
    printf("jumpifeq substring_return_null LF@_tmp bool@true\n");
    printf("jumpifeq substring_return_null LF@_index_from LF@_strlen\n");
    printf("gt LF@_tmp LF@_index_to LF@_strlen # $j > strlen($s)\n");
    printf("jumpifeq substring_return_null LF@_tmp bool@true\n");
    printf("jumpifeq substring_return_null LF@_index_to LF@_index_from\n");


    printf("# getchar <var> <symb1> <symb2>\n");
    printf("label substring_while\n");
    printf("getchar LF@_tmp LF@_string LF@_index_from\n");

    printf("concat LF@_retval LF@_retval LF@_tmp\n");

    printf("add LF@_index_from LF@_index_from int@1 # $i++\n");
    printf("jumpifneq substring_while LF@_index_from LF@_index_to\n");


    printf("jump substring_end\n");

    printf("label substring_return_null\n");
    printf("move LF@_retval nil@nil\n");

    printf("label substring_end\n");
    printf("pushs LF@_retval\n");
    printf("popframe\n");
    printf("return\n");
}

void gen_ord() {
    printf("label $ord\n");
    printf("createframe\n");
    printf("pushframe\n");

    printf("defvar LF@_string\n");
    printf("defvar LF@_retval\n");
    printf("move LF@_retval int@0\n");

    printf("defvar LF@_countArgs\n");
    printf("pops LF@_countArgs\n");
    printf("jumpifneq _TYPE_SEM_ERR LF@_countArgs int@1\n");

    printf("pops LF@_string\n");

    printf("type GF@typeCheck LF@_string\n");
    printf("jumpifneq _TYPE_SEM_ERR GF@typeCheck string@string\n");

    printf("strlen LF@_retval LF@_string\n");
    printf("jumpifeq ord_end LF@_retval int@0\n");

    printf("strlen LF@_retval LF@_string\n");
    printf("jumpifeq ord_end LF@_retval int@0\n");

    printf("stri2int LF@_retval LF@_string int@0\n");
    printf("jump ord_end\n");

    printf("label ord_end\n");
    printf("pushs LF@_retval\n");
    printf("popframe\n");
    printf("return\n");
}

void gen_chr() {
    printf("label $chr\n");
    printf("createframe\n");
    printf("pushframe\n");

    printf("defvar LF@_int\n");
    printf("defvar LF@_retval\n");

    printf("defvar LF@_countArgs\n");
    printf("pops LF@_countArgs\n");
    printf("jumpifneq _TYPE_SEM_ERR LF@_countArgs int@1\n");

    printf("pops LF@_int\n");

    printf("type GF@typeCheck LF@_int\n");
    printf("pushs GF@typeCheck\n");
    printf("pushs string@int\n");
    printf("jumpifneqs _TYPE_SEM_ERR\n");

    printf("int2char LF@_retval LF@_int\n");

    printf("label chr_end\n");
    printf("pushs LF@_retval\n");
    printf("popframe\n");
    printf("return\n");
}

void gen_builtin_functions() {
    gen_write();
    gen_reads();
    gen_readi();
    gen_readf();
    gen_strlen();
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

void gen_expr_checkType(){
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
			"jumpifeq error_sem7 LF@type_var2 string@string\n"
			"jumpifeq rel1_secondfloat LF@type_var2 string@float\n"
			"gt LF@_bool LF@_var1 LF@_var2\n"
			"jumpifeq rel2_cmp LF@_jumprel2 bool@true\n #if it is also >= or <="
			"gt LF@_bool LF@_var1 LF@_var2\n"
			"move LF@_var1 LF@_bool\n"
			"move LF@_var2 LF@_bool\n"
			"jump checkEnd\n"
			"label rel1_secondfloat\n"
			"pushs LF@_var1\n"
            "pushs int@1\n"
			"call $floatval\n"
			"pops LF@_var1\n"
			"gt LF@_bool LF@_var1 LF@_var2\n"
			"jumpifeq rel2_cmp LF@_jumprel2 bool@true\n #if it is also >= or <="
			"gt LF@_bool LF@_var1 LF@_var2\n"
			"move LF@_var1 LF@_bool\n"
			"move LF@_var2 LF@_bool\n"
			"jump checkEnd\n"

			"label rel1_firstfloat\n"
            "jumpifeq error_sem7 LF@type_var2 string@string\n"
			"pushs LF@_var2\n"
			"pushs int@1\n"
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
			"gt LF@_bool LF@_var1 LF@_var2\n"
			"jumpifeq rel2_cmp LF@_jumprel2 bool@true\n #if it is also >= or <="
			"gt LF@_bool LF@_var1 LF@_var2f\n"
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
            "pushs int@1\n"
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
            "pushs int@1\n"
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
			"jumpifeq arit2_second LF@type_var1 string@float\n"
			"jumpifneq arit2_skip1 LF@type_var1 string@int\n"
			"pushs LF@_var1\n"
            "pushs int@1\n"
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
            "pushs int@1\n"
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

void gen_expr_compute(){
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
void gen_expr_semicolon(){
    printf(
            "######SEMICOLON#####\n"
            "label exprSemicolon\n"
            "createframe\n"
            "pushframe\n"
            "defvar LF@returnVal\n"
            "pops LF@returnVal\n"
            "defvar LF@type_returnVal\n"
            "type LF@type_returnVal LF@returnVal\n"

            "jumpifneq exprSkip9 LF@type_returnVal string@bool\n"
            "write string@cannot_return_boolean\n"
            "exit int@7\n"
            "label exprSkip9\n"
            // "write LF@returnVal\n" //dbg
            "pushs LF@returnVal\n"
            "popframe\n"
            "return\n"
            );
}

void gen_expr_rightparen(){

    printf(
            "######RIGHT_PAREN#####\n"
            "label exprRightParen\n"
            "createframe\n"
            "pushframe\n"
            "defvar LF@returnVal\n"
            "pops LF@returnVal\n"
            "defvar LF@type_returnVal\n"
            "type LF@type_returnVal LF@returnVal\n"

            "jumpifeq exprDetermineBool LF@type_returnVal string@bool\n"
            "jumpifeq exprConvertNum LF@type_returnVal string@int\n"
            "jumpifeq exprConvertNum LF@type_returnVal string@float\n"
            "jumpifeq exprConvertString LF@type_returnVal string@string\n"

            "label exprConvertString\n"
            "jumpifeq exprFalse LF@returnVal string@0\n"
            "defvar LF@str_len\n"
            "strlen LF@str_len LF@returnVal\n"
            "jumpifeq exprFalse LF@str_len int@0\n"
            "jump exprTrue\n"



            "label exprConvertNum\n"
            "pushs LF@returnVal\n"
            "pushs int@1\n"
            "call $floatval\n"
            "pops LF@returnVal\n"
            "jumpifeq exprFalse LF@returnVal float@0x0p+0\n"
            "jump exprTrue\n"

            "label exprDetermineBool\n"
            "jumpifeq exprTrue LF@returnVal bool@true\n"
            "jump exprFalse\n"

            "label exprTrue\n"
            "move LF@returnVal bool@true\n"
            // "write string@true\n" // dbg
            "jump exprEnd\n"

            "label exprFalse\n"
            "move LF@returnVal bool@false\n"
            // "write string@false\n" //dbg
            "jump exprEnd\n"

            "label exprEnd\n"
            "pushs LF@returnVal\n"
            "popframe\n"
            "return\n"
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

    gen_expr_checkType();
	gen_expr_compute();
    gen_expr_semicolon();
    gen_expr_rightparen();
    gen_builtin_functions();

    printf("\nlabel _START\n");
    printf("createframe\n");
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
    } else if (!strcmp(ptr, "\"\"")) {
        return "";
    }

    while (*ptr != '\0') {
        if (*ptr == 92) { // backslash
            strPushBack(tmp, "\\092", 4);
        } else if (*ptr == 35) { // #
            strPushBack(tmp, "\\035", 4);
        } else if (*ptr <= 32) {
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

char * convertFloatToIFJ(double x) {
    char * ptr = malloc(32);
    if (ptr == NULL) {
        exit(ERR_INTERNAL);
    }

    snprintf(ptr, 32, "%a", x);

    return ptr;
}

void genFunctionLabel(htab_key_t functionName) {
    printf(
        "\njump $%s_end # genFunctionLabel() \n"
        "label $%s\n"
        "pushframe\n"
        "defvar LF@%%retval\n"
        "move LF@%%retval nil@nil\n"
        "defvar LF@%%_countArgs\n"
        "pops LF@%%_countArgs # __ \n"
    , functionName, functionName);
}

void genFunctionEnd(htab_key_t functionName) {
    printf(
        "label $%s_ret # genFunctionEnd() \n"
        "type GF@typeCheck LF@%%retval\n"
        "pushs GF@typeCheck\n"
        "pushs LF@_returnType\n"
        "jumpifneqs _TYPE_SEM_ERR\n"
        "pushs LF@%%retval\n"
        // todo check for retval with function type (exit 7 (type compatibility))
        "popframe\n"
        "return\n"
        "label $%s_end #__ \n\n"
    , functionName, functionName);
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
    printf("defvar LF@_returnType #genFunctionRetType() \n");

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
            printf("move LF@_returnType string@nil\n");
            break;
        default: // todo
            fprintf(stderr, "Error: line %d", __LINE__);
            break;
    }
}

void genTypeCheck(int count, char * varName) {
    printf("type GF@typeCheck LF@%%%s%% #genTypeCheck()\n", varName);
    printf("pushs GF@typeCheck\n");
    printf("pushs LF@_paramType%%%d\n", count-1);
    printf("jumpifneqs _TYPE_SEM_ERR #__\n");
}

void genFunctionAmountOfGivenArgsCheck(int toCompareWith) {
    printf("pushs LF@%%_countArgs\n");
    printf("pushs int@%d\n", toCompareWith);
    printf("jumpifeqs _TYPE_SEM_ERR #__\n");
}

int genFunctionPushsVariable(struct Parser * parser) {
    char * ptr = malloc(16);
    if (ptr == NULL) {
        return ERR_INTERNAL;
    }

    snprintf(ptr, 16, "LF@param%d", parser->onArg);   

    CODEGEN_INSERT_IN_DLL("pushs ", ptr);

    free(ptr);

    return 0;
}