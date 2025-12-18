#include "../board.h"
#include "../debug_io.c"

void test_algebraic() {
    char input_str[] = "e2e3 e1e2 e2e3\0";
    board _board = from_long_algebraic(input_str);

    print_board(&_board);
}