OBJS = bst.o main.o integer.o heap.o
OOPTS = -Wall -Wextra -g -c
LOPTS = -Wall -Wextra -g

all : main

main : $(OBJS)
		gcc $(LOPTS) $(OBJS) -o main

integer.o : integer.c integer.h
		gcc $(OOPTS) integer.c

bst.o : bst.c bst.h
		gcc $(OOPTS) bst.c

main.o :	main.c integer.h
		gcc $(OOPTS) main.c

sll-0-12.0 : sll-0-12.c sll-0-12.h sll.o
		gcc $(OOPTS) sll-0-12.c

heap.o : dll.c dll.h
		gcc $(OOPTS) heap.c

test : main
		./main

valgrind : main
		valgrind ./main

clean	:
		rm -f -r $(OBJS) main *.dSYM

memcheck : main
		valgrind --leak-check=yes ./main
