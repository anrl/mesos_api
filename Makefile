CC = gcc
CFLAGS = -Wall -g -ggdb

all: libmarathon.o test.o test

test: test.o libmarathon.o
	$(CC) $(CFLAGS) -o test test.o libmarathon.o -lcurl

test.o: test.c libmarathon.h
	$(CC) $(CFLAGS) test.c -c

libmarathon.o: libmarathon.c libmarathon.h
	$(CC) $(CFLAGS) libmarathon.c -c

clean:
	rm *.o test
