# Makefile for 'indexer' module                                  
#
# Bora Bozdogan, April 2025

OBJS = indexer.o indextest.o 
LIBS = ../common/common.a ../libcs50/libcs50.a -lm 

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
MAKE = make

all: indexer indextest

indexer:indexer.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest:indextest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer.o: indexer.c
	$(CC) $(CFLAGS) -c indexer.c

indextest.o: indextest.c indexer.c
	$(CC) $(CFLAGS) -c indextest.c

test: indexer indextest
	./testing.sh

valgrind: indexer
	valgrind ./testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f *.a
