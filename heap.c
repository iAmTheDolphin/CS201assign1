#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "heap.h"
#include "queue.h"
#include "bst.h"

struct heap {
    void (*display)(void *,FILE *);
    int (*compare)(void *,void *);
    void (*free)(void *);
    int maxHeap;
    BST *tree;
    QUEUE *parents;

};


HEAP *newHEAP(
        void (*display)(void *,FILE *),    //display
        int (*compare)(void *,void *),     //compare
        void (*free)(void *)) {

    HEAP *h = malloc(sizeof(HEAP));
    h->parents = newQUEUE(display, free);
    h->tree = newBST(display, compare, NULL, free);

    h->display = display;
    h->free = free;
    h->compare = compare;

    return h;
}


