#include "eval.h"
#include "search.h"

int negamax(board *brd, int depth) {
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

   BEGIN_FOREACH_MOVE(mov)
   int score = -negamax(brd, 4);
   if (score > best_score) {
      best_score = score;
      best_move = mov;
   }
   END_FOREACH_MOVE()

   search_res ans = {best_score, 5, best_move};
   return ans;
}
