CC     = gcc
CFLAGS = -g -Wall

.PHONY: all clean

all: bin/kmp bin/test_sa

bin/kmp: kmp.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

bin/sa.o: sa.c sa.h
	mkdir -p bin
	$(CC) $(CFLAGS) -c -o $@ $<

bin/test_sa: test_sa.c bin/sa.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf bin/
