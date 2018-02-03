//
// Created by Parker Jones on 2/1/18.
//

#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

struct treeNode {

    void *value;
    TREENODE *left;
    TREENODE *right;
    TREENODE *parent;

};

TREENODE *newTREENODE(TREENODE *parent, void *value) {
    TREENODE *n = malloc(sizeof(TREENODE));
    n->parent = parent;
    n->value = value;
    return n;
}

/* accessors */
TREENODE *getParent(TREENODE *n) { return n->parent; }
TREENODE *getRIGHT (TREENODE *n) { return n->right; }
TREENODE *getLeft(TREENODE *n) { return n->left; }

/* mutators */
void setLeft(TREENODE *n, TREENODE *child) { n->left = child; }
void setRight (TREENODE *n, TREENODE *child) { n->right = child; }
void setParent(TREENODE *n, TREENODE *parent) { n->parent = parent; }

void freeTREENODE(TREENODE *n, void (*release)(void *)) {
    if (release != 0 && n->value != 0) release(n->value);
    free(n);
}




int debugHEAP = 0;

struct heap {
    DLL *items;
    TREENODE *root;
    int size;

};

HEAP *newHEAP() {
    HEAP *heap = malloc(sizeof(HEAP));
    DLL *list = malloc(sizeof(DLL));
    heap->root = 0;
    heap->size = 0;
    return heap;
}

void insertHEAP(HEAP *heap, void *value) {
    TREENODE *n;
    if(heap->size == 0) {
        n = newTREENODE(0, value);
        heap->root = n;
    }
    else { 
        n = newTREENODE(getDLL(heap->items, heap->size / 2 - 1), value);
    }
    insertDLL(heap->items, heap->size, n);
    heap->size ++;
}

void displayHEAP(HEAP *heap) {
    displayDLL(heap->items, stdout);
}

