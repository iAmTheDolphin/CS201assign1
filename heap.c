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
    int size;

};


HEAP *newHEAP(
        void (*display)(void *,FILE *),    //display
        int (*compare)(void *,void *),     //compare
        void (*free)(void *)) {

    HEAP *h = malloc(sizeof(HEAP));
    assert(h != 0);
    h->parents = newQUEUE(0, 0);
    h->tree = newBST(display, compare, NULL, free);

    h->display = display;
    h->free = free;
    h->compare = compare;
    h->size = 0;

    return h;
}



/*        insert        */
void insertHEAP(HEAP *h,void *value) {
    BSTNODE *n = newBSTNODE(value);
    if(h->size == 0) {
        setBSTroot(h->tree, n);
    }
    else {
        BSTNODE *parent = dequeue(h->parents);
        setBSTNODEparent(n, parent);
        if(!getBSTNODEleft(parent)) {
            setBSTNODEleft(parent, n);
        }
        else {
            setBSTNODEright(parent, n);
        }
    }
    h->size++;
    setBSTsize(h->tree, 1);
    enqueue(h->parents, n);
    enqueue(h->parents, n);
}



/*        accessors        */
int  sizeHEAP(HEAP *h) {
    return h->size;
}

void *peekHEAP(HEAP *h) {
    return getBSTNODEvalue(getBSTroot(h->tree));
}



/*        visualizers        */

void displayHEAP(HEAP *h,FILE *fp) {
    displayBST(h->tree, fp);
}

void displayHEAPdebug(HEAP *h,FILE *fp) {
    displayBSTdebug(h->tree, fp);
}



/*        frees        */

void freeHEAP(HEAP *h) {
    freeBST(h->tree);
    freeQUEUE(h->parents);
    free(h);
}