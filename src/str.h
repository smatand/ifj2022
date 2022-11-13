#ifndef STR_H
#define STR_H

#include <stdio.h>

/** @brief Structure for string properties */
typedef struct {
    char * str;
    unsigned int realLen; // current length of the string
    unsigned int allocatedSize; // allocated size for the string
} string_t;

/**
 * @brief Allocates and initializes a string structure
 * @param string to operate with
 * 
 * @return SUCCESS, otherwise ERR_INTERNAL
 */
string_t * stringInit(int * ret);

/**
 * @brief Destroys given string structure
 */
void stringDestroy(string_t * s);

/**
 * @brief Clears the string up to the 1st null character
 * 
 * Useful for clearing after converting to int/decimals
 */
void stringClear(string_t * s);

/**
 * @brief Looks ahead by one characters
 * @param fp file pointer
 * 
 * @return character
 */
int lookAheadByOneChar(FILE * fp);

/**
 * @brief Resizes string
 * @param s struct to operate with
 * @param toSize new size
 * 
 * @return SUCCESS, otherwise ERR_CODE
 */
int stringResize(string_t * s, int toSize);

/**
 * @brief Pushes character to the end of the string
 * @param s struct to operate with
 * @param c char being pushed to the end
 * 
 * @return SUCCESS, otherwise ERR_INTERNAL
 */
int charPushBack(string_t * s, int c);

/**
 * @brief Pushes multiple characters to the end of the string
 * @param s struct to operate with
 * @param source pointer to chars being pushed to the string
 * @param len length of string being pushed
 * 
 * @return SUCCESS, otherwise ERR_INTERNAL
 */
int strPushBack(string_t * s, char * source, int len);

#endif // STR_H