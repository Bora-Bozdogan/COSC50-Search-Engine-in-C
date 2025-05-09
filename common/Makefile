# Makefile for common library
#   Builds and compiles all these utility functions into a library.
#
# Bora Bozdogan, April 2025

# object files, and the target library
OBJS = pagedir.o index.o word.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I../libcs50 -I../common
CC = gcc
MAKE = make

# Build $(LIB) by archiving object files
# (you will need to drop in copy of set.c, counters.c, hashtable.c)
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.c pagedir.h index.c word.c

index.o: pagedir.c pagedir.h index.c word.c

word.o: pagedir.c pagedir.h index.c word.c

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
# (this rule is used only by the Professor in preparing the starter kit)
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f $(LIB) *~ *.o
	rm -f *.a
