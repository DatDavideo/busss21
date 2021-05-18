.PHONY: all clean
CC=gcc
CFLAGS=-std=c11 -pedantic -Wall -Werror -fsanitize=address,undefined

all: listen

clean:
	rm -rf listen listen.o

listen: listen.o
	$(CC) $(CFLAGS) -o $@ $<

listen.o: listen.c
	$(CC) $(CFLAGS) -c $^
