CC = clang++
STD = -std=c++11
TEST_LFLAGS = -lboost_unit_test_framework

tree: tree.h option.h tree_main.cpp
	$(CC) $(STD) -o tree tree_main.cpp

test: tree.h option.h test_main.cpp
	$(CC) $(STD) $(TEST_LFLAGS) -o test test_main.cpp

all: tree test

clean:
	-rm tree test
