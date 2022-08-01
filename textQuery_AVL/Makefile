CC = gcc
CFLAGS = -Wall -Werror -g
CNOERR = -Wall -g

# NOTE:
# None of your .c files should contain a main() function
# List all your supporting C files here
# For example: SUPPORTING_FILES = hello.c world.c
SUPPORTING_FILES = AVL.c textQuery.c wordRecord.c



.PHONY: all
all: prog

prog: main.c AVL.c textQuery.c wordRecord.c
	$(CC) $(CNOERR) -o prog main.c AVL.c textQuery.c wordRecord.c




.PHONY: clean
clean:
	rm -rf prog
