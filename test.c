#include "lib/unity.h"
#include "tests/test_board.c"

void setUp() {}

void tearDown() {}

int main() {
   UNITY_BEGIN();
   RUN_TEST(set_up_board);
   return UNITY_END();
}
