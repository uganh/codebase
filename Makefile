CC     = gcc
CFLAGS = -g -Wall

.PHONY: all clean

all: kmp

kmp: kmp.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f kmp
