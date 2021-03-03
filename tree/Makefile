CC     = gcc
CFLAGS = -g -Wall

.PHONY: all clean

all: tree treap

tree: tree.c
	$(CC) $(CFLAGS) -o $@ $^

treap: treap.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f tree treap
