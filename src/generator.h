#ifndef GENERATOR_H
#define GENERATOR_H

/**
 * @brief Initialization of code generator
*/
void genInit();

/**************** STANDARD LIBRARY (PHP) ****************/
void gen_reads();
void gen_readi();
void gen_readf();
void gen_write();
void gen_floatval();
void gen_intval();
void gen_strval();
void gen_strlen(/*string*/);
void gen_substring(/*string, int, int*/);



#endif // GENERATOR_H