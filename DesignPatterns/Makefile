CXX      = c++
CXXFLAGS = -g -Wall -std=c++11

TARGETS = Singleton Visitor

all: $(TARGETS)

.PHONY: all clean

$(TARGETS): %: %.cc
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf $(TARGETS)
