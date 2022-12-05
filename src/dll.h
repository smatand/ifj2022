#ifndef DLL_H
#define DLL_H

typedef struct DLLElement {
    char * data;

    struct DLLElement * previousElem;
    struct DLLElement * nextElem;
} DLLElement_t;

typedef struct DLList {
    DLLElement_t * firstElem;
    DLLElement_t * lastElem;
} DLList_t;

/**
 * @brief Initialization of DLLlist
 * @param list pointer to the list
 */
void DLLInit(DLList_t * list);

/**
 * @brief Disposement of DLLlist
 * @param list pointer to the list
 */
void DLLDispose(DLList_t * list);

/**
 * @brief Insertion of element to the end of the list
 * @param list pointer to the list
 * @param data data being inserted to list
 * @param size size of the data
 * @param elem after to store
 * @return 0 if success, ERR_INTERNAL if error
 */
int DLLInsertLast(DLList_t * list, char * dataToInsert, int size);



int DLLInsertBefore(DLList_t * list, char * dataToInsert, int size, DLLElement_t * elem);

/**
 * @brief Print the list
 * @param list to print
 */
void DLLPrintAll(DLList_t * list);

void DLLPrintAllReversed(DLList_t * list);

#endif // DLL_H