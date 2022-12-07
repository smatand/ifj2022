#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dll.h"
#include "error.h"

void DLLInit(DLList_t * list) {
    list->firstElem = NULL;
    list->lastElem = NULL;
}

void DLLDispose(DLList_t * list) {
    DLLElement_t * i = list->firstElem;

    while (i != NULL) {
        list->firstElem = list->firstElem->nextElem;
        free(i->data);
        i->data = NULL;
        free(i);
        i = NULL;
        i = list->firstElem;
    }

    list->lastElem = NULL;
}

int DLLInsertLast(DLList_t * list, char * dataToInsert, int size) {
    DLLElement_t * elem = malloc(sizeof(DLLElement_t));
    if (elem == NULL) {
        return ERR_INTERNAL;
    }

    elem->data = malloc(size);
    if (elem->data == NULL) {
        free(elem);
        return ERR_INTERNAL;
    }

    memcpy(elem->data, dataToInsert, size);
    elem->nextElem = NULL;
    elem->previousElem = list->lastElem;

    // if list is empty
    if (list->firstElem == NULL) {
        list->firstElem = elem;
    } else {
        list->lastElem->nextElem = elem;
    }

    list->lastElem = elem; // update last element

    return 0; // SUCCESS
}

void DLLPrintAll(DLList_t * list) {
    DLLElement_t * i = list->firstElem;

    while (i != NULL) {
        printf("%s\n", i->data);

        i = i->nextElem;
    }
}

void DLLPrintAllReversed(DLList_t * list) {
    DLLElement_t * i = list->lastElem;

    while (i != NULL) {
        printf("%s\n", i->data);

        i = i->previousElem;
    }
}