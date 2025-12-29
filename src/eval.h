#include "board.h"
#include "movegen.h"

#ifndef eval_h
#define eval_h

// pawn, knight, bishop, rook, queen, king
enum materials {
   PAWN_VAL = 100,
   KNIGHT_VAL = 300,
   BISHOP_VAL = 300,
   ROOK_VAL = 500,
   QUEEN_VAL = 900,
   KING_VAL = 100'000
};

#endif

uint32_t eval(board *brd);
