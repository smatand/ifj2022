#ifndef GENERATOR_H
#define GENERATOR_H

/**************** STANDARD LIBRARY (PHP) ****************/

/** @brief function reads() : ?string */
void gen_reads();

/** @brief function readi() : ?int */
void gen_readi();

/** @brief function readf() : ?float */
void gen_readf();

/** @brief function write(term1, term2, ..., termn) : void */
void gen_write();

/** @brief function floatval(term) : ?float */
void gen_floatval();

/** @brief function intval(term) : ?int */
void gen_intval();

/** @brief function strval(term) : ?string */
void gen_strval();

void gen_strlen(/*string*/);
void gen_substring(/*string, int, int*/);

/** @brief Prints the standard library (IFJ2022) to the output file */
void gen_builtin_functions();

/************** END OF STANDARD LIBRARY (PHP) ************/

/** @brief Initialization of code generator*/
void genInit();

#endif // GENERATOR_H