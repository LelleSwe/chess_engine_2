#include "eval.h"
#include "search.h"

static uint64_t total_nodes = 0;
static uint64_t nodes_since_last = 0;
static uint64_t nps = 0;
static search_res ans = {
    .score = 0, .depth = 0, .nodes = 0, .nps = 0, .time = 0, .mov = 0};

static struct timespec timer;
static struct timespec start;

double time_pass(struct timespec *start, struct timespec *end) {
   double taken =
       (end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec) / 1e9;
   return taken;
}

void print_res(search_res *res) {
   printf("info score cp %d depth %d nps %lu nodes %lu time %lu\n", res->score,
          res->depth, res->nps, res->nodes, res->time);
}

void update_nps() {
   struct timespec cur_time;

   timespec_get(&cur_time, TIME_UTC);
   double taken = time_pass(&timer, &cur_time);
   nps = (uint64_t)((double)nodes_since_last / taken);
   nodes_since_last = 0;
   timer = cur_time;
}

int negate_score(int score) {
   if (score < -32000) // decay the checkmate score by one ply.
      score++;

   return -score;
}

int negamax_alphabeta(board *brd, int alpha, int beta, int depth) {
   int max = CHECKMATE;
   if (total_nodes % (1 << 22) == 0) {
      update_nps();
      print_res(&ans);
      nodes_since_last = 0;
      if (time_pass(&start, &timer) > 5) {
         return -eval(brd);
      }
   }
   total_nodes++;
   nodes_since_last++;

   if (depth == 0) {
      return eval(brd);
   }

   BEGIN_FOREACH_MOVE(mov)
   int score = negate_score(negamax_alphabeta(brd, -beta, -alpha, depth - 1));

   if (score > max) {
      max = score;
      if (score > alpha)
         alpha = score;
      if (score >= beta) {
         undo_move(brd, mov);
         return max;
      }
   }

   END_FOREACH_MOVE()
   if (max == CHECKMATE) {
      return prev_wasnt_legal(brd, brd->to_play) ? CHECKMATE : 0;
   }

   return max;
}

search_res search(board *brd) {
   total_nodes = 0;
   nodes_since_last = 0;
   nps = 0;
   timespec_get(&timer, TIME_UTC);
   timespec_get(&start, TIME_UTC);

   int depth = 0;

   while (time_pass(&start, &timer) < 5) {
      move best_move = 0;
      int best_score = CHECKMATE;

      BEGIN_FOREACH_MOVE(mov)
      int score =
          negate_score(negamax_alphabeta(brd, best_score, -best_score, depth));
      if (score > best_score) {
         best_score = score;
         best_move = mov;
      }
      END_FOREACH_MOVE()

      depth++;
      update_nps();
      ans.mov = best_move;
      ans.depth = depth;
      ans.nodes = total_nodes;
      ans.nps = nps;
      ans.score = best_score;
      ans.time = (uint64_t)(time_pass(&start, &timer) * 1000);
      print_res(&ans);
   }

   return ans;
}
