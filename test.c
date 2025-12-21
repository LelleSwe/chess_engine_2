#include "lib/unity.h"
#include "tests/test_board.c"

void setUp() {}

void tearDown() {}

int main() {
   int test_board = run_test_board();
   return test_board;
}
