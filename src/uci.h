#include "board.h"

void *start_uci(void *argp);
void uci(board *brd, vec_move *moves);

#ifndef ptharg_def
#define ptharg_def
typedef struct {
   board *brd;
   vec_move *moves;
} pthargs;
#endif
