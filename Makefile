clean:
	rm -rf *.o
	rm -rf testsymtab

list: symtable.h symtablelist.o
	gcc -Wall -ansi -pedantic -o testsymtab testsymtab.c symtablelist.o

symtablelist.o: symtable.h symtablelist.c
		gcc -c symtablelist.c

hash: symtable.h symtablehash.o
	  gcc -Wall -ansi -pedantic -o testsymtab testsymtab.c symtablehash.o

symtablehash.o: symtable.h symtablehash.c
		gcc -c symtablehash.c
