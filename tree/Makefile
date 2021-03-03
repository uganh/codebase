CC     = gcc
CFLAGS = -g -Wall

.PHONY: all clean

all: bin/tree bin/treap bin/splaytree

bin/tree: tree.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

bin/treap: treap.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

bin/splaytree: splaytree.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf bin/
