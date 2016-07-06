CC = gcc
CFLAGS = -Wall -g -ggdb

all: marathon.o test.o test

test: test.o marathon.o
	$(CC) $(CFLAGS) -o test test.o marathon.o -lcurl

test.o: test.c marathon.h
	$(CC) $(CFLAGS) test.c -c

marathon.o: marathon.c marathon.h
	$(CC) $(CFLAGS) marathon.c -c

clean:
	rm *.o test
