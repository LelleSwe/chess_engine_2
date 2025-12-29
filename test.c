#include "lib/unity.h"
#include "tests/test_board.c"
#include "tests/test_movegen.c"

void setUp() {}

void tearDown() {}

int main() {
   int test_board = run_test_board();
   int test_movegen = run_test_movegen();
   return test_board | test_movegen;
}
