#ifndef __INTEGER_INCLUDED__
#define __INTEGER_INCLUDED__

#include <stdio.h>

typedef struct INTEGER INTEGER;

extern INTEGER *newINTEGER(int);
extern int getINTEGER(INTEGER *);
extern int setINTEGER(INTEGER *,int);
extern int compareINTEGERmax(void *,void *);
extern int compareINTEGERmin(void *,void *);
extern void displayINTEGER(void *,FILE *);
extern void freeINTEGER(void *);

#endif
