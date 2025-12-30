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
   uint64_t nodes;
   uint64_t nps;
   uint64_t time;
   move mov;
} search_res;

#endif
#define CHECKMATE -100'000

search_res search(board *brd);
int negamax_alphabeta(board *brd, int alpha, int beta, int depth);
void print_res(search_res *res);
