# Macros
CC = gcc217
# CC = gcc217m
CFLAGS =
# CFLAGS = -g
# CFLAGS = -D NDEBUG
# CFLAGS = -D NDEBUG -O

# Dependency Rules for Non-File Targets
all: testsymtablelist testsymtablehash
clobber: clean
	rm -f *~ \#*\#
clean:
	rm -f testsymtablelist testsymtablehash *.o

# Dependency Rules for file targets
testsymtablelist: testsymtablelist.o symtablelist.o
	$(CC) $(CFLAGS) testsymtablelist.o symtablelist.o -o testsymtablelist

testsymtablehash: testsymtablehash.o symtablehash.O
	$(CC) $(CFLAGS) testsymtablehash.o symtablehash.o -o testsymtablehash

testsymtablelist.o: testsymtablelist.c symtablelist.h 
	$(CC) $(CFLAGS) -c testsymtablelist.c

testsymtablehash.o: testsymtablehash.c symtablehash.h
	$(CC) $(CFLAGS) -c testsymtablehash.c

symtablelist.o: symtablelist.c symtablelist.h
	$(CC) $(CFLAGS) -c symtablelist.c

symtablehash.o: symtablehash.c symtablehash.h
	$(CC) $(CFLAGS) -c symtablehash.c