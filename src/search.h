#include "board.h"

#ifndef search_h
#define search_h
typedef struct {
   int score;
   int depth;
} score_depth;

typedef struct {
   int score;
   int depth;
   move mov;
} search_res;

#endif

search_res search(board *brd);
int negamax_alphabeta(board *brd, int alpha, int beta, int depth);
