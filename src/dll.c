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
    DLLElement_t * elem = malloc(sizeof(DLLElement_t)); // todo: possible leak? IAL
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

//int DLLInsertAfter(DLList_t * list, char * dataToInsert, int size, DLLElement_t * elem) {
//    DLLElement_t * newElem = malloc(sizeof(DLLElement_t));
//    if (newElem == NULL) {
//        return ERR_INTERNAL;
//    }
//
//    newElem->data = malloc(size);
//    if (newElem->data == NULL) {
//        free(newElem);
//        return ERR_INTERNAL;
//    }
//
//    memcpy(newElem->data, dataToInsert, size);
//
//    if (elem == NULL) {
//
//    }
//    newElem->nextElem = elem->nextElem;
//
//    //newElem->nextElem = elem->nextElem;
//    //newElem->previousElem = elem;
//
//    if (elem->nextElem != NULL) {
//        elem->nextElem->previousElem = newElem;
//    } else {
//        list->lastElem = newElem;
//    }
//
//    elem->nextElem = newElem;
//
//    return 0;
//}
//
//int DLLInsertBefore(DLList_t * list, char * dataToInsert, int size, DLLElement_t * elem) {
//    DLLElement_t * newElem = malloc(sizeof(DLLElement_t));
//    if (newElem == NULL) {
//        return ERR_INTERNAL;
//    }
//
//    newElem->data = malloc(size);
//    if (newElem->data == NULL) {
//        free(newElem);
//        return ERR_INTERNAL;
//    }
//
//    // insert before elem
//    memcpy(newElem->data, dataToInsert, size);
//    newElem->nextElem = elem;
//
//    if (elem->previousElem != NULL) {
//        elem->previousElem->nextElem = newElem;
//    } else {
//        list->firstElem = newElem;
//    }
//
//    newElem->previousElem = elem->previousElem;
//    elem->previousElem = newElem;
//
//    return 0;
//}