#include "str.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LEN 16


int stringInit(string_t * s) {
    s = calloc(1, sizeof(string_t));
    if (s == NULL) {
        fprintf(stderr, "Memory allocation of string struct failed");
        return ERR_INTERNAL;
    }

    s->str = malloc(DEFAULT_LEN);
    if (s->str == NULL) {
        fprintf(stderr, "Memory allocation of string failed");
        return ERR_INTERNAL;
    }

    s->allocatedSize = DEFAULT_LEN;
    s->realLen = 0;
    s->str[s->realLen] = '\0';

    return SUCCESS;
}

void stringDestroy(string_t * s) {
    free(s->str);
    s->realLen = 0;
    s->allocatedSize = 0;
}