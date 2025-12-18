CC = gcc
CFLAGS = -I. -I./src -std=c23 -O3 -march=native -Wall -Wpedantic -Wextra -Wfloat-equal -Wundef -Wshadow
SRC = $(wildcard src/*.c) $(wildcard src/*.h)
TEST = $(wildcard test/*.c) $(wildcard test/*.h)

.PHONY: all main test test_uci clean

EXEC_OUT = target/main
TEST_OUT = target/test

all: clean main test

main: clean $(SRC)
	$(CC) $(SRC) $(CFLAGS) main.c -o $(EXEC_OUT)

test: clean $(SRC) $(TEST) main
	$(CC) $(SRC) $(TEST) $(CFLAGS) test.c -o $(TEST_OUT)
	./target/test
	$(MAKE) test_uci

test_uci: 
	./fastchess/fastchess --compliance ./target/main

clean:
	rm -f target/*
