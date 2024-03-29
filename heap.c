#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "heap.h"
#include "queue.h"
#include "stack.h"
#include "bst.h"


int debugHEAP = 0;

struct heap {
    void (*display)(void *, FILE *);

    int (*compare)(void *, void *);

    void (*free)(void *);

    int maxHeap;
    BST *tree;
    QUEUE *parents;
    STACK *interior;
    int size;
    STACK *lastAdded;

};


HEAP *newHEAP(
        void (*display)(void *, FILE *),    //display
        int (*compare)(void *, void *),     //compare
        void (*free)(void *)) {

    HEAP *h = malloc(sizeof(HEAP));
    assert(h != 0);
    h->parents = newQUEUE(0, 0);
    h->interior = newSTACK(0, 0);
    h->lastAdded = newSTACK(0,0);
    h->tree = newBST(display, compare, NULL, free);

    h->display = display;
    h->free = free;
    h->compare = compare;
    h->size = 0;

    return h;
}


/*        insert        */

void insertHEAP(HEAP *h, void *value) {
    BSTNODE *n = newBSTNODE(value);
    if (h->size == 0) {
        setBSTroot(h->tree, n);
    } else {
        BSTNODE *parent = dequeue(h->parents);
        setBSTNODEparent(n, parent);
        if (!getBSTNODEleft(parent)) {
            setBSTNODEleft(parent, n);
            push(h->interior, parent);
        } else {
            setBSTNODEright(parent, n);
        }
    }
    h->size++;
    setBSTsize(h->tree, h->size);
    enqueue(h->parents, n);
    enqueue(h->parents, n);
    push(h->lastAdded, n);
}


/*        accessors        */
int sizeHEAP(HEAP *h) {
    return h->size;
}

void *peekHEAP(HEAP *h) {
    return getBSTNODEvalue(getBSTroot(h->tree));
}


/*        visualizers        */

void displayHEAP(HEAP *h, FILE *fp) {
    displayBST(h->tree, fp);
}

void displayHEAPdebug(HEAP *h, FILE *fp) {
    printf("heap size: %d\nbst size: %d\n", h->size, sizeBST(h->tree));
    displayBSTdebug(h->tree, fp);
}


/*        frees        */

void freeHEAP(HEAP *h) {
    freeBST(h->tree);
    freeQUEUE(h->parents);
    freeSTACK(h->interior);
    freeSTACK(h->lastAdded);
    free(h);
}


static void swap(BSTNODE *n1, BSTNODE *n2) {
    void *data = getBSTNODEvalue(n1);
    setBSTNODEvalue(n1, getBSTNODEvalue(n2));
    setBSTNODEvalue(n2, data);
}




//MIN OR MAX HEAP IS DETERMINED BY THE COMPARATOR PASSED IN NOT INSIDE THE HEAP

//HEAP HAS NO NOTION OF WHAT IS A MIN OR A MAX HEAP


/*        builder        */

static BSTNODE *getMaxChild(HEAP *h, BSTNODE *parent) {
    if (getBSTNODEright(parent) &&
        h->compare(getBSTNODEvalue(getBSTNODEleft(parent)), getBSTNODEvalue(getBSTNODEright(parent))) > 0) {
        // if the right is bigger
        return getBSTNODEright(parent);
    } else {
        return getBSTNODEleft(parent);
    }
}

static void heapify(HEAP *h, BSTNODE *n) {
    //this needs to check down and bubble down
    //if it is a parent at all, it will have a left child
    if (debugHEAP) printf("heapify\n");
    if (getBSTNODEright(n)) {
        BSTNODE *maxChild = getMaxChild(h, n);
        if (h->compare(getBSTNODEvalue(maxChild), getBSTNODEvalue(n)) < 0) {
            if(debugHEAP) printf ("swapping...\n");
            swap(n, maxChild);
            heapify(h, maxChild);
        }
    } else if (getBSTNODEleft(n)) {
        if (h->compare(getBSTNODEvalue(getBSTNODEleft(n)), getBSTNODEvalue(n)) < 0) {
            if(debugHEAP) printf ("swapping...\n");
            swap(n, getBSTNODEleft(n));
            heapify(h, getBSTNODEleft(n));
        }
    }
}

void buildHEAP(HEAP *h) {
    int stackSize = sizeSTACK(h->interior);

    if (debugHEAP) {
        printf("Building heap with %d interior nodes\n", sizeSTACK(h->interior));
        printf("\n");
    }


    for (int x = 0; x < stackSize; x++) {
        if (debugHEAP) {
            h->display(getBSTNODEvalue(peekSTACK(h->interior)), stdout);
            printf("  -- building heap... nodes left in stack: %d  ---%d\n",sizeSTACK(h->interior), x);
        }
        heapify(h, pop(h->interior));
    }

}




/*        extract        */

void *extractHEAP(HEAP *h) {
    BSTNODE *n = pop(h->lastAdded);
    BSTNODE *root = getBSTroot(h->tree);
    void *data = getBSTNODEvalue(root);
    swap(n, root);
    pruneLeafBST(h->tree, n);
    heapify(h, getBSTroot(h->tree));
    freeBSTNODE(n, 0);
    h->size--;
    return data;

}
