//
// Created by Parker Jones on 2/14/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include "string.h"

struct String {
    char *value;
};

STRING *
newSTRING(char *x) {
    STRING *p = malloc(sizeof(STRING));
    assert(p != 0);
    p->value = x;
    return p;
}

char
*getSTRING(STRING *v) {
    return v->value;
}

char
*setSTRING(STRING *v, char *x) {
    char *old = v->value;
    v->value = x;
    return old;
}

void
displaySTRING(void *v, FILE *fp) {
    fprintf(fp, "%s", getSTRING((STRING *) v));
}

int
compareSTRINGmax(void *v, void *w) {
    int x = strcmp(v, w);
    if(x) {
        return x > 0 ? -1 : 1;
    }
    else {
        return 0;
    }
}

int
compareSTRINGmin(void *v, void *w) {
    return strcmp(v, w);
}

void
freeSTRING(void *v) {
    free((STRING *) v);
}
