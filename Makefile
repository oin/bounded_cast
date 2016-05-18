.PHONY: all run clean
all: run

run: test
	./test

test: test.cpp numeric_domain.hpp
	$(CXX) -std=c++11 -Wall -O3 -o $@ $<

clean:
	rm -f test