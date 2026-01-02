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

// total available time + increment
extern int64_t btime;
extern int64_t wtime;
extern int64_t binc;
extern int64_t winc;
extern int64_t movestogo;
extern int64_t movetime;
extern int64_t searchdepth;
#endif
#define CHECKMATE -100'000

search_res search(board *brd);
int negamax_alphabeta_ibv(board *brd, int alpha, int beta, int depth);
void print_res(search_res *res);
