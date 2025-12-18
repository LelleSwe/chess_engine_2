#include "../lib/unity.h"
#include "../src/board.h"
#include "../src/debug_io.h"

// void test_algebraic() {
//    char input_str[] = "e2e3 e1e2 e2e3\0";
//    board _board = from_long_algebraic(input_str);
//
//    print_board(&_board);
// }

void set_up_board() {
   board brd = gen_start_board();
   int equal = board_cmp(&default_start_board, &brd);
   TEST_ASSERT(equal);
}
