CC = gcc
CFLAGS = -I. -I./src -std=c23 -O3 -march=native -Wall -Wpedantic -Wextra -Wfloat-equal -Wundef -Wshadow
SRC = $(wildcard src/*.c) $(wildcard src/*.h)
TEST = $(wildcard test/*.c) $(wildcard test/*.h)
LIB = $(wildcard lib/*.c) $(wildcard lib/*.h)

.PHONY: all main test test_uci clean install perft

EXEC_OUT = target/main
TEST_OUT = target/test

all: install main test

main: $(SRC)
	$(CC) $(SRC) $(CFLAGS) main.c -o $(EXEC_OUT)

# -lunity -I/usr/include/unity /usr/lib/libunity.a 
test: $(SRC) $(TEST) main
	$(MAKE) unit
	$(MAKE) uci
	$(MAKE) perft

unit: $(SRC) $(TEST)
	$(CC) $(LIB) $(SRC) $(TEST) $(CFLAGS) test.c -o $(TEST_OUT)
	./target/test

uci: main
	./fastchess/fastchess --compliance ./target/main

# testing tools
install:
	if [ ! -d "fastchess" ]; then \
		git clone https://github.com/Disservin/fastchess.git; \
		cd fastchess; \
		make; \
		cd ..; \
	fi
	if [ ! -e "./lib/unity.h" ]; then \
		cd lib; \
		wget https://raw.githubusercontent.com/ThrowTheSwitch/Unity/refs/heads/master/src/unity.c; \
		wget https://raw.githubusercontent.com/ThrowTheSwitch/Unity/refs/heads/master/src/unity.h; \
		wget https://raw.githubusercontent.com/ThrowTheSwitch/Unity/refs/heads/master/src/unity_internals.h; \
		cd ..; \
	fi
	if [ ! -e "standard.epd" ]; then \
		wget https://raw.githubusercontent.com/AndyGrant/Ethereal/refs/heads/master/src/perft/standard.epd; \
	fi
	if [ ! -e "perft.py" ]; then \
		wget https://raw.githubusercontent.com/AndyGrant/Ethereal/refs/heads/master/src/perft/perft.py; \
	fi

perft : main
	python perft.py --depth 6 ./target/main standard.epd 
# > perft_res.txt

clean:
	rm -f target/*
