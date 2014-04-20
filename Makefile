# My own hand-rolled Makefile
# Copyright (C) 2014  Vernon R. Jones
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


# Definitions

CC = clang++
STD = -std=c++11
TEST_LFLAGS = -lboost_unit_test_framework


# Recipes

tree: tree.h option.h tree_main.cpp
	$(CC) $(STD) -o tree tree_main.cpp

test: tree.h option.h test_main.cpp
	$(CC) $(STD) $(TEST_LFLAGS) -o test test_main.cpp

all: tree test

clean:
	-rm tree test
