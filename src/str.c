#include "str.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>

#define DEFAULT_LEN 16


string_t * stringInit(int * ret) {
    string_t * s = calloc(1, sizeof(string_t));
    if (s == NULL) {
        fprintf(stderr, "Memory allocation of string_t failed\n");

        ret = ERR_INTERNAL;;
        return NULL;
    }

    s->str = calloc(1, DEFAULT_LEN);
    if (s->str == NULL) {
        fprintf(stderr, "Memory allocation of string failed\n");

        free(s);
        ret = ERR_INTERNAL;
        return NULL;
    }

    s->allocatedSize = DEFAULT_LEN;
    s->realLen = 0;
    s->str[s->realLen] = '\0';

    return s;
}

void stringDestroy(string_t * s) {
    free(s->str);
    s->realLen = 0;
    s->allocatedSize = 0;
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
        return ERR_INTERNAL;
    }

    s->allocatedSize = toSize; // incrementing by DEFAULT_LEN

    return SUCCESS;
}

int charPushBack(string_t * s, int c) {
    if (s->realLen == s->allocatedSize - 1) {
        s->str = realloc(s->str, s->realLen + DEFAULT_LEN);
        if (s->str == NULL) {
            return ERR_INTERNAL;
        }
        s->allocatedSize = s->realLen + DEFAULT_LEN; // incrementing by DEFAULT_LEN
    }

    s->str[s->realLen++] = c;
    s->str[s->realLen] = '\0';

    return SUCCESS;
}