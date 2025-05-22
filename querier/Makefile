# Makefile for 'querier' module                                  
#
# Bora Bozdogan, April 2025

OBJS = querier.o
LIBS = ../common/common.a ../libcs50/libcs50.a -lm 

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
MAKE = make

all: querier

querier:querier.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer.o: querier.c
	$(CC) $(CFLAGS) -c querier.c

test: querier
	./testing.sh

valgrind: querier
	valgrind ./testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f *.a
