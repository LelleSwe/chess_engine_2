#include "eval.h"
#include "search.h"

// statistics
static uint64_t total_nodes = 0;
static uint64_t nodes_since_last = 0;
static uint64_t nps = 0;
static search_res ans = {
    .score = 0, .depth = 0, .nodes = 0, .nps = 0, .time = 0, .mov = 0};

// time controls
static struct timespec timer;
static struct timespec start;
static bool do_search = false;
int64_t btime = 0;
int64_t wtime = 0;
int64_t binc = 0;
int64_t winc = 0;
int64_t movestogo = 0;
int64_t movetime = 0;
int64_t searchdepth = -1;
static int64_t search_time = 0;
static int64_t search_soft_deadline = 0;
static int64_t search_hard_deadline = 0;

#define max(x, y)                                                              \
   ({                                                                          \
      auto res1 = (x);                                                         \
      auto res2 = (y);                                                         \
      (res1 > res2 ? res1 : res2);                                             \
   })

double time_pass(struct timespec *start, struct timespec *end) {
   double taken =
       (end->tv_sec - start->tv_sec) + (end->tv_nsec - start->tv_nsec) / 1e9;
   return taken;
}

int64_t timespec_to_int(struct timespec *time) {
   return (int64_t)(time->tv_sec * 1'000.0 + time->tv_nsec / 1'000'000.0);
}

// decently ugly to just throw around statics
void calc_move_time() {
   timespec_get(&start, TIME_UTC);

   int64_t comb_time = btime | wtime;
   int64_t comb_inc = winc | binc;

   if (!comb_time && !movetime && (searchdepth < 0))
      searchdepth = 6;

   if (comb_time && !movetime) {
      if (movestogo > 0) {
         search_time = comb_time / max(2, movestogo) - 50;
         if (search_time < 10)
            search_time = 10;
      } else {
         search_time = comb_time / 20 + comb_inc / 2;
      }
   }
   if (search_time) {
      search_soft_deadline = timespec_to_int(&start) + search_time / 3;
      search_hard_deadline = timespec_to_int(&start) + search_time;
   } else {
      search_soft_deadline = 0;
      search_hard_deadline = 0;
   }
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
   // decay the checkmate score by one ply.
   if (score < -32000 * 4)
      score += 4;

   return -score;
}

bool node_probe() {
   if (total_nodes % (1 << 19) == 0) {
      update_nps();
      print_res(&ans);
      nodes_since_last = 0;
      timespec_get(&timer, TIME_UTC);
      if (search_hard_deadline > 0 &&
          timespec_to_int(&timer) > search_hard_deadline) {
         do_search = false;
         return true;
      }
   }
   return false;
}

int force_exact(int score) { return (score + 1) & ~3; }
int force_lower(int score) { return force_exact(score) + 1; }

int negamax_alphabeta_ibv(board *brd, int alpha, int beta, int depth) {
   int max = CHECKMATE * 4; // -100'000

   if (!do_search || node_probe())
      return force_lower(alpha);
   total_nodes++;
   nodes_since_last++;

   zobrist hash = *arr_last_zobrist4096(&brd->zobrist_history);
   if (tt_exists(hash)) {
      tt_entry *entry = tt_get(hash);

      if (entry->depth >= depth) {

         int score4 = entry->score & 3; // mod 4
         if (score4 == 1 && entry->score >= beta)
            return entry->score;
         if (score4 == 3 && entry->score <= alpha)
            return entry->score;
         if (score4 == 0)
            return entry->score;
         // printf("info string hash %lu entry depth %d our depth %d eval mod
         // "
         //        "4 %d\n",
         //        hash, entry->depth, depth, entry->score % 4);
      }
   }

   if (depth == 0) {
      // 4x for ibv search
      return 4 * eval(brd);
   }
   move best_move;

   int alpha_start = alpha;
   int cut = force_exact(beta);
   BEGIN_FOREACH_MOVE(mov) {
      int score =
          negate_score(negamax_alphabeta_ibv(brd, -beta, -alpha, depth - 1));

      if (score > max) {
         max = score;
         best_move = mov;
         if (score > alpha)
            alpha = score;
         if (score >= cut) {
            tt_set(hash, best_move, force_lower(max), depth);

            undo_move(brd, mov);
            return force_lower(max);
         }
      }
   }
   END_FOREACH_MOVE()

   if (max == 4 * CHECKMATE) {
      // printf("yes? \n");
      max = prev_wasnt_legal(brd, !brd->to_play) ? 4 * CHECKMATE : 0;
   }

   if (max <= force_exact(alpha_start)) {
      max = force_exact(max) - 1; // high bound
   } else if (max >= force_exact(beta)) {
      max = force_lower(max); // lower bound
   } else {
      max = force_exact(max); // exact
   }

   tt_set(hash, best_move, max, depth);
   // printf("saved hash %lu best move %d ibv score %d depth %d to tt\n", hash,
   //        best_move, max, depth);
   return max;
}

search_res search(board *brd) {
   do_search = true;
   total_nodes = 0;
   nodes_since_last = 0;
   nps = 0;
   timespec_get(&timer, TIME_UTC);
   timespec_get(&start, TIME_UTC);
   calc_move_time();
   printf("info string search_time=%ld\n", search_time);
   printf("info string current time =%ld\n", timespec_to_int(&start));
   printf("info string search_soft_deadline=%ld\n", search_soft_deadline);
   printf("info string search_hard_deadline=%ld\n", search_hard_deadline);
   printf("info string searchdepth=%ld\n", searchdepth);

   int depth = searchdepth > 0 ? searchdepth : 0;

   printf("info string diff?? %ld\n",
          (timespec_to_int(&timer) - search_soft_deadline));
   while (do_search) {
      move best_move = 0;
      int best_score = CHECKMATE;

      if (searchdepth >= 0 && depth > searchdepth) {
         do_search = false;
         break;
      }
      if (search_soft_deadline > 0 &&
          timespec_to_int(&timer) > search_soft_deadline) {
         do_search = false;
         break;
      }

      BEGIN_FOREACH_MOVE(mov)
      int score = negate_score(
          negamax_alphabeta_ibv(brd, CHECKMATE, -best_score, depth));
      score /= 4; // ibv score
      if (score > best_score) {
         best_score = score;
         best_move = mov;
      }
      END_FOREACH_MOVE()

      depth++;
      if (do_search) {
         update_nps();
         ans.mov = best_move;
         ans.depth = depth;
         ans.nodes = total_nodes;
         ans.nps = nps;
         ans.score = best_score;
         ans.time = (uint64_t)(time_pass(&start, &timer) * 1000);
      }
      print_res(&ans);
   }

   depth = 0;
   wtime = 0;
   btime = 0;
   winc = 0;
   binc = 0;
   movetime = 0;
   movestogo = 0;
   return ans;
}
