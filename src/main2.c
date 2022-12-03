#include <stdio.h>
#include <stdlib.h>
#include "generator.h"
#include "scanner.h"
#include "dll.h"

int main() {
    DLList_t * list = malloc(sizeof(DLList_t));
    if (list == NULL) {
        return 1;
    }
    DLLInit(list);

    token_t * token = tokenInit();

    int ret = scanToken(token);
    if (ret) {
        return ret;
    }
    gen_write(token, list); // nacitat string
    stringDestroy(token->attribute.strVal);

    ret = scanToken(token);
    gen_write(token, list);

    ret = scanToken(token);
    gen_write(token, list);

    DLLPrintAll(list);

    DLLDispose(list);
    free(list);
    freeToken(token);


    return 0;
}
