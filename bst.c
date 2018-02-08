//
// Created by Parker Jones on 2/4/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bst.h"

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

static void backupSwapperBST(BSTNODE *n1, BSTNODE *n2) {
    void *data1 = getBSTNODEvalue(n1);
    setBSTNODEvalue(n1, getBSTNODEvalue(n2));
    setBSTNODEvalue(n2, data1);
}

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

BSTNODE *getBSTroot(BST *t) {
    return t->root;
}

void setBSTroot(BST *t,BSTNODE *replacement) {
    t->root = replacement; //TODO - make this move around parents/children?
}

void setBSTsize(BST *t,int s) {
    t->size = s; //FIXME - WHY IS THIS HERE?
}

static BSTNODE *insertHelper(BST *t, BSTNODE *n, void *value) {
    BSTNODE *n2;
    if (t->comparator(getBSTNODEvalue(n), value) <= 0) { //value is bigger or equal
        if(getBSTNODEright(n)){
            n2 = insertHelper(t, getBSTNODEright(n), value);
        }
        else {
            n2 = newBSTNODE(value);
            setBSTNODEright(n, n2);
        }
    }
    else { //value is less than
        if(getBSTNODEleft(n)) {
            n2 = insertHelper(t, getBSTNODEleft(n), value);
        }
        else {
            n2 = newBSTNODE(value);
            setBSTNODEleft(n, n2);
        }
    }
    return n2;
}

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


void pruneLeafBST(BST *t,BSTNODE *leaf) {
    BSTNODE *parent = getBSTNODEparent(leaf);
    if(parent != 0) {
        if (getBSTNODEleft(parent) == leaf) {
            setBSTNODEleft(parent, 0);
        } else {
            setBSTNODEright(parent, 0);
        }
    } //TODO - might need to free the leaf. right now assuming that calling has a handle
    t->size--;
}

int sizeBST(BST *t) {
    return t->size;
}

void statisticsBST(BST *t,FILE *fp) {
    printf("Nodes: %d\n", t->size);
    printf("Minimum depth: %d", t->size);

}


void displayHelper(BST *t, BSTNODE *n, FILE *fp) {
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
void displayBST(BST *t,FILE *fp) {
    if(t->size == 0) {
        printf("[]");
    }
    else {
        displayHelper(t, t->root, fp);
    }
}