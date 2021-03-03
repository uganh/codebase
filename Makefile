CC     = gcc
CFLAGS = -g -Wall

.PHONY: all clean

all: bin/kmp

bin/kmp: kmp.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf bin/
