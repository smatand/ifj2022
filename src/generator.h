#ifndef GENERATOR_H
#define GENERATOR_H

#include "scanner.h"
#include "dll.h"
#include "str.h"
#include "parser.h"

typedef struct Parser Parser_t;

#define CODEGEN_INSERT_IN_DLL(str1, str2)           \
    do {                                            \
        int ret = 0;                                \
        string_t * tmp = stringInit(&ret);          \
        if (ret) {                                  \
            return ret;                             \
        }                                           \
                                                    \
        strPushBack(tmp, str1, strlen(str1));       \
        strPushBack(tmp, str2, strlen(str2));       \
                                                    \
        if (DLLInsertLast(parser->codeGen, tmp->str, tmp->realLen+1) != SUCCESS) \
            return ERR_INTERNAL;                    \
        stringDestroy(tmp);                         \
    } while (0)

/**************** STANDARD LIBRARY (PHP) ****************/

/** @brief function reads() : ?string */
void gen_reads();

/** @brief function readi() : ?int */
void gen_readi();

/** @brief function readf() : ?float */
void gen_readf();

/** @brief function write(term1, term2, ..., termn) : void 
 * 
 * @param token to parse its type and generate formatted string
 * @param list to insert instruction of ifjcode22
 * @return ERR_INTERNAL in case of malloc failed
 */
void gen_write();

/** @brief function floatval(term) : ?float */
void gen_floatval();

/** @brief function intval(term) : ?int */
void gen_intval();

/** @brief function strval(term) : ?string */
void gen_strval();

/** @brief function substring(string $s, int $i, int $j) : ?string */
void gen_substring(/*string, int, int*/);

/** @brief function ord(string $c) : int */
void gen_ord();

/** @brief function chr(int $i) : string */
void gen_chr();

/** @brief Prints the standard library (IFJ2022) to the output file */
void gen_builtin_functions();

/************** END OF STANDARD LIBRARY (PHP) ************/


/** @brief Convert from C string to IFJcode22 string
 * 
 * @param str C string
 * @return IFJcode22 string
 */
char * convertStringToIFJ(char * str);

/** @brief Convert from C int to IFJcode22 string
 * 
 * @param str C int
 * @return IFJcode22 string
*/
char * convertIntToIFJ(int x);

/** @brief Convert from C float to IFJcode22 string
 * 
 * @param str C float
 * @return IFJcode22 string
*/
char * convertFloatToIFJ(double x);


/** @brief generating code for main, function calls, definitions */
void gen_checkType();
void gen_compute();
void genInit();
void genEnd();
void genFunctionEnd();
void genFunctionLabel(htab_key_t functionName);
void genToPush(char * toPush);
void genFunctionParamType(keyword_t kw, int count);
void genFunctionRetType(keyword_t kw);
void genTypeCheck(int count, char * varName);
void genFunctionAmountOfGivenArgsCheck(int toCompareWith);
void genFunctionAmountOfParamsCheck(int count);
int genFunctionPushsVariable(struct Parser * parser);

#endif // GENERATOR_H
