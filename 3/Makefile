.PHONY: all clean
CC=gcc
CFLAGS=-std=c11 -pedantic -Wall -Werror -fsanitize=address,undefined

all: bend

clean:
	rm -rf bend bend.o

bend: bend.o
	$(CC) $(CFLAGS) -o $@ $<

bend.o: bend.c
	$(CC) $(CFLAGS) -c $^

