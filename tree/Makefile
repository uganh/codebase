CC      = gcc
CFLAGS  = -g -Wall

TARGETS = tree treap splaytree

all: $(TARGETS)

.PHONY: all clean

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(TARGETS)
