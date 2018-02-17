#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "bst.h"
#include "heap.h"
#include "scanner.h"
#include "real.h"
#include "string.h"
#include "integer.h"

#include <unistd.h>

/* options */
int Reals = 0;
int Strings = 0;
int Increasing = 1;
int debugMAIN = 1;
char *filename;


static int processOptions(int,char **);
void Fatal(char *,...);


int endCommand = 0;

int
main(int argc,char **argv)
{
    printf("Running\n");

    FILE *fp;

    int argIndex;

    if (argc == 1) Fatal("%d arguments!\n",argc-1);

    argIndex = processOptions(argc,argv);

    if(endCommand) return 0;

    if (argIndex == argc)
        printf("No arguments\n");
    else
    {
        filename = argv[argIndex];

        if(debugMAIN) {
            printf("Remaining arguments:\n");
            for (int i = argIndex; i < argc; ++i)
                printf("    %s\n", argv[i]);
        }
    }

    if(debugMAIN) printf("Trying to open %s ", filename);
    fp = fopen(filename, "r");
    if(!fp && debugMAIN) printf("couldn't open file!\n");
    else if(debugMAIN) printf("opened file!\n");



    HEAP *h;
    void (*display)(void *, FILE *);
    void (*free)(void *);

    if(Reals) { //it is a file of reals
        if(Increasing) h = newHEAP(displayREAL, compareREALmin, freeREAL);
        else  h = newHEAP(displayREAL, compareREALmax, freeREAL);
        double temp = readReal(fp);
        while(!feof(fp)) {
            insertHEAP(h, newREAL(temp));
            temp = readReal(fp);
        }
        buildHEAP(h);
        display = displayREAL;
        free = freeREAL;
    }
    else if(Strings) { //it is a file of strings
        if(Increasing) h = newHEAP(displaySTRING, compareSTRINGmin, freeSTRING);
        else h = newHEAP(displaySTRING, compareSTRINGmax, freeSTRING);
        char *temp;
        if(stringPending(fp)) {
            temp = readString(fp);
        }
        else {
            temp = readToken(fp);
        }
        while(!feof(fp)) {
            insertHEAP(h, newSTRING(temp));
            if(stringPending(fp)) {
                temp = readString(fp);
            }
            else {
                temp = readToken(fp);
            }
        }
        buildHEAP(h);
        display = displaySTRING;
        free = freeSTRING;
    }
    else { //it is an integer file
        if(Increasing) h = newHEAP(displayINTEGER, compareINTEGERmin, freeINTEGER);
        else h = newHEAP(displayINTEGER, compareINTEGERmax, freeINTEGER);
        int temp = readInt(fp);
        while (!feof(fp)) {
            insertHEAP(h, newINTEGER(temp));
            temp = readInt(fp);
        }
        buildHEAP(h);
        display = displayINTEGER;
        free = freeINTEGER;
    }


    //printing out the sorted list
    while(sizeHEAP(h) > 0) {
        if(debugMAIN) {
            printf("\n");
            displayHEAPdebug(h, stdout);
            printf("\n");
        }
        void *value = extractHEAP(h);
        display(value, stdout);
        if(sizeHEAP(h) != 0) printf(" ");
        free(value);
    }
    printf("\n");

    freeHEAP(h);

    return 0;
}




void Fatal(char *fmt, ...)
{
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
}


void explain () {
    printf("Written by Parker Jones\n"
                   "My insertHeap took constant time because I kept a queue of available parents\n"
                   "which meant I could assign a new node immediately to the next available parent.\n"
                   "\n"
                   "My buildHeap took linear time because I used a stack of all interior nodes and \n"
                   "had to call heapify on each one of them. Heapify is O(log(n)) in the worst case,\n"
                   "but on average, it rarely needs to recur, and only a very small fraction of \n"
                   "values would already be far from the leaves\n"
                   "\n"
                   "My extractHeap takes O(log(n)) time because when you remove the root node of the\n"
                   "heap, the last inserted leaf takes its place and must then be heapified.\n"
                   "Heapify takes log(n) in the worst case because the value must trickle all the way\n"
                   "down from the root to a leaf.\n");
}


/* only -oXXX  or -o XXX options */

static int
processOptions(int argc, char **argv)
{
    int argIndex;
    int argUsed;
    int separateArg;
    argIndex = 1;


    while (argIndex < argc && *argv[argIndex] == '-')
    {
        if(debugMAIN) printf("Checking Options\n");

        /* check if stdin, represented by "-" is an argument */
        /* if so, the end of options has been reached */
        if (argv[argIndex][1] == '\0') return argIndex;

        separateArg = 0;
        argUsed = 0;

        if (argv[argIndex][2] == '\0')
        {
            //filename = argv[argIndex+1];
            separateArg = 1;
        }
            //filename = argv[argIndex]+2;

        switch (argv[argIndex][1])
        {
            /*
             * when option has an argument, do this
             *
             *     examples are -m4096 or -m 4096
             *
             *     case 'm':
             *         MemorySize = atol(arg);
             *         argUsed = 1;
             *         break;
             *
             *
             * when option does not have an argument, do this
             *
             *     example is -a
             *
             *     case 'a':
             *         PrintActions = 1;
             *         break;
             */

            case 'v':
                explain();
                endCommand = 1;
                break;
            case 'i':
                argUsed = 1;
                break;
            case 'I':
                Increasing = 1;
                argUsed = 1;
                break;
            case 'D':
                Increasing = 0;
                argUsed = 1;
                break;
            case 's':
                Strings = 1;
                Reals = 0;
                argUsed = 1;
                break;
            case 'r':
                Reals = 1;
                Strings = 0;
                argUsed = 1;
                break;
            default:
                Fatal("option %s not understood\n",argv[argIndex]);
        }

        if (separateArg && argUsed){
            if(debugMAIN) printf("separate arg and argUsed\n");
            //++argIndex;
        }

        ++argIndex;
    }
    if(debugMAIN) printf("%d\n", argIndex);

    return argIndex;
}
