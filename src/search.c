#include "eval.h"
#include "search.h"

// static uint64_t nodes = 0;

int negamax(board *brd, int depth) {
   // nodes++;
   if (depth == 0) {
      return eval(brd);
   }

   int max = INT32_MIN;
   BEGIN_FOREACH_MOVE(mov)
   int score = -negamax(brd, depth - 1);
   if (score > max)
      max = score;

   END_FOREACH_MOVE()

   return max;
}

search_res search(board *brd) {
   move best_move = 0;
   int best_score = INT32_MIN;

   // CLOCK_START();
   BEGIN_FOREACH_MOVE(mov)
   int score = -negamax(brd, 5);
   if (score > best_score) {
      best_score = score;
      best_move = mov;
   }
   END_FOREACH_MOVE()

   // CLOCK_END();
   // printf("nps: %.9f\n", (double)nodes / TIME_TAKEN);

   search_res ans = {best_score, 5, best_move};
   return ans;
}
