CC = clang++
STD = -std=c++11
TEST_LFLAGS = -lboost_unit_test_framework

tree: tree.cpp tree.h option.h
	$(CC) $(STD) -o tree tree.cpp

test: tree.h option.h test_main.cpp
	$(CC) $(STD) $(TEST_LFLAGS) -o test test_main.cpp

all: tree test

clean:
	-rm tree test
