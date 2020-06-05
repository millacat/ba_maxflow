# 'make' with no arguments executes the first rule in the file.
CC=gcc
# -Werror: Make all warnings into errors.
# -Wall turns on a Lot of warning flags
# -Wextra: This enables some extra warning flags that are not enabled by -Wall.
# -pedantic: Issue all the warnings demanded by strict ISO C. Follows the
# version of the ISO C standard specified by any -std option used. 
# -g: Produce debugging information in the operating system’s native format
# (stabs, COFF, XCOFF, or DWARF). GDB can work with this debugging information. 
CFLAGS=-std=c11 -Wall -Wextra -pedantic -g -Wno-unused-but-set-variable

# by writing main.c in next line make knows when main.c is changed
# it needs to run the rule 'main'
graphs: main.c 
	$(CC) $(CFLAGS) container.c list.c edge.c queue.c graph.c maxflow.c pushrelabel.c loaddimacs.c main.c -o max.out -lprocps
	
clean:
	rm -f max.out

# makes sure make doesn't do anything to a file named 'clean'
.PHONY: clean

