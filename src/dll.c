#include <string.h>
#include <stdlib.h>

#include "dll.h"
#include "error.h"

void DLLInit(DLList_t * list) {
    list->firstElem = NULL;
    list->lastElem = NULL;
}

void DLLDispose(DLList_t * list) {
    DLLElement_t * tmp = list->firstElem;

    while (list->firstElem != NULL) {
        list->firstElem = list->firstElem->nextElem;
        free(tmp->data);
        tmp->data = NULL;
        free(tmp);
        tmp = NULL;
        tmp = list->firstElem;
    }

    list->lastElem = NULL;
}

int DLLInsertLast(DLList_t * list, char * dataToInsert, int size) {
    DLLElement_t * elem = malloc(sizeof(struct DLLElement)); // todo: possible leak? IAL
    if (elem == NULL) {
        return ERR_INTERNAL; // exit(ERR_INTERNAL)
    }

    elem->data = malloc(size);
    if (elem->data == NULL) {
        free(elem);
        return ERR_INTERNAL; // exit(ERR_INTERNAL)
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