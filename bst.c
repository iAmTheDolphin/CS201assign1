//
// Created by Parker Jones on 2/4/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bst.h"
#include "queue.h"

//region BSTNODE

struct bstnode {
    void *value;
    BSTNODE *left;
    BSTNODE *right;
    BSTNODE *parent;
};

BSTNODE *newBSTNODE(void *value) {
    BSTNODE *n = malloc(sizeof(BSTNODE));
    n->value = value;
    n->parent = 0;
    n->left = 0;
    n->right = 0;
    return n;
}

void *getBSTNODEvalue(BSTNODE *n) {
    return n->value;
}

void setBSTNODEvalue(BSTNODE *n,void *value) {
    free(n->value); //TODO check back in on this.
    n->value = value;
}

BSTNODE *getBSTNODEleft(BSTNODE *n) {
    return n->left;
}

void setBSTNODEleft(BSTNODE *n,BSTNODE *replacement) {
    n->left = replacement; //TODO - check back in on this for mem leaks
}

BSTNODE *getBSTNODEright(BSTNODE *n) {
    return n->right;
}

void setBSTNODEright(BSTNODE *n,BSTNODE *replacement) {
    n->right = replacement; //TODO - check back in on this for mem leaks
}

BSTNODE *getBSTNODEparent(BSTNODE *n) {
    return n->parent;
}

void setBSTNODEparent(BSTNODE *n,BSTNODE *replacement) {
    n->parent = replacement;
}

void freeBSTNODE(BSTNODE *n,void (*f)(void *)) {
    if (f != 0) {  f(n->value); }
    free(n);
}
//endregion


struct bst {
    void (*display)(void *s,FILE *fp) ;
    int (*comparator)(void *v1,void *v2);
    void (*swapper)(BSTNODE *n1,BSTNODE *n2);
    void (*free)(void *v);
    BSTNODE *root;
    int size;

    //TODO MORE VALUES TO COME

};

//this is a default swapper method in case the one given is null
static void backupSwapperBST(BSTNODE *n1, BSTNODE *n2) {
    void *data1 = getBSTNODEvalue(n1);
    setBSTNODEvalue(n1, getBSTNODEvalue(n2));
    setBSTNODEvalue(n2, data1);
}



/*
The constructor is passed four functions, one that knows how to display the generic value
 stored in a node, one that can compare two generic values, one that knows how to swap t
 he two generic values held by BSTNODEs (the swapper function is used by swapToLeafBST),
 and one that knows how to free a generic value. If the swapper function is NULL, then the
 constructor should store its own swapper function.
*/
BST *newBST(
        void (*display)(void *,FILE *),           //display
        int (*compatator)(void *,void *),            //comparator
        void (*swapper)(BSTNODE *,BSTNODE *),     //swapper
        void (*free)(void *)) {

    BST *tree = malloc(sizeof(BST));
    assert(tree != 0);
    tree->free = free;
    tree->comparator = compatator;
    tree->display = display;
    if(swapper != 0) {
        tree->swapper = swapper;
    }
    else {
        tree->swapper = backupSwapperBST;
    }
    tree->root = 0;
    tree->size = 0;
    return tree;
}



/*        helpers        */

static BSTNODE *insertHelper(BST *t, BSTNODE *n, void *value) {
    BSTNODE *n2;
    if (t->comparator(getBSTNODEvalue(n), value) <= 0) { //value is bigger or equal
        if(getBSTNODEright(n)){
            n2 = insertHelper(t, getBSTNODEright(n), value);
        }
        else {
            n2 = newBSTNODE(value);
            if(getBSTNODEright(n2)) printf("SCRREEEEEEEE\n");
            setBSTNODEright(n, n2);
        }
    }
    else { //value is less than
        if(getBSTNODEleft(n)) {
            n2 = insertHelper(t, getBSTNODEleft(n), value);
        }
        else {
            n2 = newBSTNODE(value);
            if(getBSTNODEleft(n2)) printf("SCRREEEEEEEE\n");
            setBSTNODEleft(n, n2);
        }
    }
    return n2;
}

static int isRightChild(BSTNODE *leaf) {
    if (getBSTNODEright(leaf->parent) == leaf) {
        return 1;
    }
    return 0;
}

static int isLeftChild(BSTNODE *leaf) {
    if(getBSTNODEleft(leaf->parent) == leaf) {
        return 1;
    }
    return 0;
}

static void displayHelper(BST *t, BSTNODE *n, FILE *fp) {
    printf("[");
    t->display(getBSTNODEvalue(n),fp);
    if(getBSTNODEleft(n)) {
        printf(" ");
        displayHelper(t, getBSTNODEleft(n), fp);
    }
    if(getBSTNODEright(n)) {
        printf(" ");
        displayHelper(t, getBSTNODEright(n), fp);
    }
    printf("]");
}

static void freeHelper(BST *t, BSTNODE *n) {
    if(getBSTNODEleft(n)) {
        freeHelper(t, getBSTNODEleft(n));
    }
    if(getBSTNODEright(n)) {
        freeHelper(t, getBSTNODEright(n));
    }
    t->free(getBSTNODEvalue(n));
    free(n);
}



/*        accessors        */

BSTNODE *getBSTroot(BST *t) {
    return t->root;
}

//This method returns the number of nodes currently in the tree.
// It should run in amortized constant time.
int sizeBST(BST *t) {
    return t->size;
}




/*        mutators        */

//This method updates the root pointer of a BST object. It should run in constant time.
void setBSTroot(BST *t,BSTNODE *replacement) {
    t->root = replacement; //TODO - make this move around parents/children?
}

void setBSTsize(BST *t,int s) {
    t->size = s; //FIXME - WHY IS THIS HERE?
}




/*        inserts        */

//This method inserts a value into the search tree, returning the inserted BSTNODE.
BSTNODE *insertBST (BST *t, void *value) {
    BSTNODE *newN;
    if(t->root == 0) {
        newN = newBSTNODE(value);
        setBSTroot(t, newN);
    }
    else {
        newN = insertHelper(t, t->root, value);
    }
    t->size ++;
    return newN;
}




/*        removers        */

void pruneLeafBST(BST *t,BSTNODE *leaf) {
    if(t->size > 1) {
        if(isLeftChild(leaf)) {
            setBSTNODEleft(leaf->parent, 0);
            t->size--;
        }
        else if (isRightChild(leaf)) {
            setBSTNODEright(leaf->parent, 0);
            t->size--;
        }
    }
    else if(t->size == 1){
        t->root = 0;
        t->size--;
    }
}



/*        visualizers        */

void displayBST(BST *t,FILE *fp) {
    if(t->size == 0) {
        printf("[]");
    }
    else {
        displayHelper(t, t->root, fp);
    }
    printf("\n");
}

void displayBSTdebug(BST *t,FILE *fp) {
    BSTNODE *n = t->root;
    QUEUE *items = newQUEUE(t->display, t->free);
    if(n) enqueue(items, n);
    int nodes = sizeQUEUE(items);

    //loops through the whole list
    while (nodes > 0) {

        //loops through the nodes at the level
        while(nodes > 0) {
            //pulls out the next node to work with
            n = peekQUEUE(items);
            t->display(getBSTNODEvalue(n), stdout);
            //dequeues the node just displayed
            dequeue(items);
            //if there is a left BSTNODE to the current node we enqueue it
            if(getBSTNODEleft(n) != 0) enqueue(items, getBSTNODEleft(n));
            //if there is a left BSTNODE to the current node we enqueue it
            if(getBSTNODEright(n) != 0) enqueue(items, getBSTNODEright(n));
            //decrement nodes left in current level
            nodes--;
            if(nodes > 0) printf(" ");
        }
        //loop ends when there are no nodes left in current level
        //set nodes to the amount of nodes in the next level.
        nodes = sizeQUEUE(items);
        printf("\n");
    }
}



/*        frees        */
void freeBST(BST *t) {
    if(t->free != NULL && t->size > 0) {
        freeHelper(t, t->root);
    }
    free(t);
}