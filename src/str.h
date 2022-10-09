#ifndef STR_H
#define STR_H

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
string_t * stringInit(int ret);

/**
 * @brief Destroys given string structure
 */
void stringDestroy(string_t * s);

#endif // STR_H