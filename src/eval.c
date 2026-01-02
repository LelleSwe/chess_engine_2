#include "eval.h"
#include "pesto_tables.h"
#include <assert.h>

#define GET_PIECES(w)                                                          \
   pawn = brd->w##pa_bb;                                                       \
   bishop = brd->w##bi_bb;                                                     \
   king = brd->w##ki_bb;                                                       \
   queen = brd->w##qu_bb;                                                      \
   rook = brd->w##ro_bb;                                                       \
   knight = brd->w##kn_bb;                                                     \
   do {                                                                        \
   } while (0);

#define LOOP_PESTO(type, TYPE)                                                 \
   while (type != 0) {                                                         \
      square sq = bitscan_lsb(type);                                           \
      type &= type - 1;                                                        \
      sq = 8 * (sq / 8) + (((-sq) % 8) + 8) % 8;                               \
      assert(0 <= sq && sq < 64);                                              \
      if (to_play) {                                                           \
         mg[1] += mg_##type##_table[sq] + mg_value[TYPE];                      \
         eg[1] += eg_##type##_table[sq] + eg_value[TYPE];                      \
      } else {                                                                 \
         mg[0] += mg_##type##_table[sq ^ 56] + mg_value[TYPE];                 \
         eg[0] += eg_##type##_table[sq ^ 56] + eg_value[TYPE];                 \
      }                                                                        \
   }                                                                           \
   do {                                                                        \
   } while (0)
// based on CPW
// https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function
int pesto_eval(board *brd) {
   // [1] for black, [0] for white
   int mg[2] = {};
   int eg[2] = {};
   int phase = 0;
   phase += bit_count(brd->wpa_bb | brd->bpa_bb) * game_phase[PAWN];
   phase += bit_count(brd->wbi_bb | brd->bbi_bb) * game_phase[BISHOP];
   phase += bit_count(brd->wro_bb | brd->bro_bb) * game_phase[ROOK];
   phase += bit_count(brd->wqu_bb | brd->bqu_bb) * game_phase[QUEEN];
   phase += bit_count(brd->wkn_bb | brd->bkn_bb) * game_phase[KNIGHT];

   bitboa pawn;
   bitboa bishop;
   bitboa king;
   bitboa queen;
   bitboa rook;
   bitboa knight;

   GET_PIECES(b);
   bool to_play = true;
   LOOP_PESTO(pawn, PAWN);
   LOOP_PESTO(bishop, BISHOP);
   LOOP_PESTO(king, KING);
   LOOP_PESTO(knight, KNIGHT);
   LOOP_PESTO(rook, ROOK);
   LOOP_PESTO(queen, QUEEN);

   GET_PIECES(w);
   to_play = false;
   LOOP_PESTO(pawn, PAWN);
   LOOP_PESTO(bishop, BISHOP);
   LOOP_PESTO(king, KING);
   LOOP_PESTO(knight, KNIGHT);
   LOOP_PESTO(rook, ROOK);
   LOOP_PESTO(queen, QUEEN);

   int mg_phase = phase;
   int eg_phase = 24 - phase;
   int score_w = mg_phase * mg[0] + eg_phase * eg[0];
   int score_b = mg_phase * mg[1] + eg_phase * eg[1];
   int material =
       ((mg[0] - mg[1]) * phase + (eg[0] - eg[1]) * (24 - phase)) / 24;
   return (score_w - score_b) / (24) + material;
}
#undef GET_PIECES
#undef LOOP_PESTO

int eval(board *brd) {
   int pesto = pesto_eval(brd);

   int score = pesto;
   // always get score positive relative to current player,
   // needed for negamax
   return brd->to_play ? -score : score;
}
