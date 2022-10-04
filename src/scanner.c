/**
 * @file scanner.c
 *
 * @brief Implementation of scanner module for IFJ22
 */

#include "scanner.h"
#include <stdio.h>
#include <ctype.h>

struct String
{
    char *data;
    size_t currLen;
    size_t memSize;
};
