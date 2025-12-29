#include "board.h"

void print_board(const board *_board);
void fprint_board(FILE *f, const board *_board);
void print_move(const move *_move);
void print_move_short(FILE *f, const move mov, const uint64_t perft_count);
void print_move_history(board *brd);
void print_board_internal(const board *brd);
void print_bitboard(const bitboa brd);
