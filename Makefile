CC       = gcc
CXX		 = c++
CFLAGS   = -g -Wall
CXXFLAGS = $(CFLAGS) -std=c++11

TARGETS  = kmp

all: $(TARGETS) test_sa test_sort

.PHONY: all test clean

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) -o $@ $^

sa.o: sa.c sa.h
	$(CC) $(CFLAGS) -c $<

test_sa: test_sa.c sa.o
	$(CC) $(CFLAGS) -o test_sa $^

test_sort: test_sort.cc sort.h heap.h
	$(CXX) $(CXXFLAGS) -o $@ $<

test: test_sa test_sort
	./test_sa
	./test_sort

clean:
	rm -rf $(TARGETS) *.o test_sa test_sort
