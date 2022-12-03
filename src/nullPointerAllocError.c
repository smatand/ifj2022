#include "dll.h"
#include <string.h>
#include <stdlib.h>

int main() {
    DLList_t * l = malloc(sizeof(DLList_t));

    DLLInit(l);

    DLLInsertLast(l, "skuska", 1+strlen("skuska"));
    DLLPrintAll(l);

    DLLDispose(l);
    free(l);
}
