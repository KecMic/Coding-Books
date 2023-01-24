# =======================================================================================================
# book_1
#  $@		target
#  $<		target's 1st dependency
#  $^		all dependencies of target
#
# verbose linker:	-Wl,--verbose
# =======================================================================================================

CXX      := g++# g++, clang++
CXXFLAGS := -std=c++17 -O0 -Wall -Wextra -Wpedantic # for debug-info: -g

.PHONY: all
all: book_1

book_1: book_1.cpp ./Makefile
	$(CXX) $(CXXFLAGS) book_1.cpp -o $@ -lpthread

.PHONY: clean
clean:
	rm -f ./book_1
