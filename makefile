OBJS = bst.o heapsort.o integer.o heap.o queue.o sll.o stack.o dll.o real.o scanner.o string.o
OOPTS = -Wall -Wextra -g -c
LOPTS = -Wall -Wextra -g
EXEC = ./heapsort -r hs-0-0.data

all : heapsort

heapsort : $(OBJS)
		gcc $(LOPTS) $(OBJS) -o heapsort

string.o : string.c string.h
		gcc $(OOPTS) string.c

scanner.o : scanner.c scanner.h
		gcc $(OOPTS) scanner.c

real.o : real.c real.h
		gcc $(OOPTS) real.c

integer.o : integer.c integer.h
		gcc $(OOPTS) integer.c

sll.o : sll.c sll.h
		gcc $(OOPTS) sll.c

queue.o : queue.c queue.h sll.o
		gcc $(OOPTS) queue.c

bst.o : bst.c bst.h
		gcc $(OOPTS) bst.c

heapsort.o :	heapsort.c integer.o real.o string.o heap.o scanner.o
		gcc $(OOPTS) heapsort.c

heap.o : heap.c heap.h queue.o stack.o
		gcc $(OOPTS) heap.c

stack.o : stack.c stack.h dll.o
		gcc $(OOPTS) stack.c

dll.o : dll.c dll.h
		gcc $(OOPTS) dll.c

test : heapsort
		$(EXEC)

valgrind : heapsort
		valgrind $(EXEC)

clean	:
		rm -f -r $(OBJS) heapsort *.dSYM

memcheck : heapsort
		valgrind --leak-check=yes $(EXEC)
