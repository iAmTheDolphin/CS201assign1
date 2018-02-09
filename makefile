OBJS = bst.o main.o integer.o heap.o queue.o sll.o
OOPTS = -Wall -Wextra -g -c
LOPTS = -Wall -Wextra -g

all : main

main : $(OBJS)
		gcc $(LOPTS) $(OBJS) -o main

integer.o : integer.c integer.h
		gcc $(OOPTS) integer.c

queue.o : queue.c queue.h sll.o
		gcc $(OOPTS) queue.c

bst.o : bst.c bst.h
		gcc $(OOPTS) bst.c

main.o :	main.c integer.h
		gcc $(OOPTS) main.c

heap.o : heap.c heap.h queue.o
		gcc $(OOPTS) heap.c

test : main
		./main

valgrind : main
		valgrind ./main

clean	:
		rm -f -r $(OBJS) main *.dSYM

memcheck : main
		valgrind --leak-check=yes ./main
