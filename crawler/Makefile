# Makefile for 'crawler' module                                  
#
# Bora Bozdogan, April 2025

OBJS = crawler.o ../common/pagedir.o 
LIBS = ../libcs50/libcs50.a -lm 

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
MAKE = make

crawler:$(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: crawler.c
	$(CC) $(CFLAGS) -c crawler.c

test: crawler
	./testing.sh

valgrind: crawler
	valgrind ./testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f *.a
