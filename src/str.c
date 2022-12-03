#include "str.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>

#define DEFAULT_LEN 16


string_t * stringInit(int * ret) {
    string_t * s = calloc(1, sizeof(string_t));
    if (s == NULL) {

        *ret = ERR_INTERNAL;;
        return NULL;
    }

    s->str = calloc(1, DEFAULT_LEN);
    if (s->str == NULL) {
        fprintf(stderr, "Memory allocation of string failed\n");

        free(s);
        *ret = ERR_INTERNAL;
        return NULL;
    }

    s->allocatedSize = DEFAULT_LEN;
    s->realLen = 0;
    s->str[s->realLen] = '\0';

    return s;
}

void stringDestroy(string_t * s) {
    s->realLen = 0;
    s->allocatedSize = 0;

    free(s->str);
    s->str = NULL;
    free(s);
    s = NULL;
}

void stringClear(string_t * s) {
    memset(s->str, 0, s->allocatedSize);
    s->realLen = 0;
}

int lookAheadByOneChar(FILE * fp) {
    int c = 0;
    c = getc(fp);

    int ret = c; // EOF or any char
    ungetc(c, fp);

    return ret;
}

int stringResize(string_t * s, int toSize) {
    s->str = realloc(s->str, toSize);
    if (s->str == NULL) {
        stringDestroy(s); // if realloc fails, deallocating string
        return ERR_INTERNAL;
    }

    s->allocatedSize = toSize; // incrementing by DEFAULT_LEN

    return SUCCESS;
}

int charPushBack(string_t * s, int c) {
    if (s->realLen == s->allocatedSize - 1) { // memory for string is full
        if (stringResize(s, s->allocatedSize + DEFAULT_LEN) != SUCCESS) {
            return ERR_INTERNAL;
        }
    }

    s->str[s->realLen++] = c;
    s->str[s->realLen] = '\0';

    return SUCCESS;
}

int strPushBack(string_t * s, char * source, int len) {
    for (int i = 0; i < len; i++){
        // charPushBack checks for full memory internally
        if (charPushBack(s, source[i]) != SUCCESS) {
            return ERR_INTERNAL;
        }
    }

    return SUCCESS;
}