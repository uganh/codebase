CC      = gcc
CFLAGS  = -g -Wall

OBJECTS = linalg.o

all: liblinalg.so test_linalg

.PHONY: all clean test

liblinalg.so: $(OBJECTS)
	$(CC) -shared -o $@ $^

$(OBJECTS): %.o: %.c
	$(CC) -fPIC -c $(CFLAGS) $^

test_linalg: test_linalg.c liblinalg.so linalg.h
	$(CC) $(CFLAGS) -o $@ $^

test: test_linalg
	@ ./test_linalg

clean:
	- rm -rf *.o *.so test_linalg
