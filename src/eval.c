#include "eval.h"
#include "pesto_tables.h"
#include <assert.h>

int material_eval(board *brd) {
   int white =
       bit_count(brd->wpa_bb) * PAWN_VAL + bit_count(brd->wkn_bb) * KNIGHT_VAL +
       bit_count(brd->wbi_bb) * BISHOP_VAL +
       bit_count(brd->wqu_bb) * QUEEN_VAL + bit_count(brd->wro_bb) * ROOK_VAL;
   int black =
       bit_count(brd->bpa_bb) * PAWN_VAL + bit_count(brd->bkn_bb) * KNIGHT_VAL +
       bit_count(brd->bbi_bb) * BISHOP_VAL +
       bit_count(brd->bqu_bb) * QUEEN_VAL + bit_count(brd->bro_bb) * ROOK_VAL;

   return white - black;
}

#define GET_PIECES(w)                                                          \
   pawn = brd->w##pa_bb;                                                       \
   bishop = brd->w##bi_bb;                                                     \
   king = brd->w##ki_bb;                                                       \
   queen = brd->w##qu_bb;                                                      \
   rook = brd->w##ro_bb;                                                       \
   knight = brd->w##kn_bb;                                                     \
   do {                                                                        \
   } while (0);

#define LOOP_PESTO(type)                                                       \
   while (type != 0) {                                                         \
      square sq = bitscan_lsb(type);                                           \
      type &= type - 1;                                                        \
      sq = 8 * (sq / 8) + (((-sq) % 8) + 8) % 8;                               \
      assert(0 <= sq && sq < 64);                                              \
      if (to_play) {                                                           \
         mg[1] += mg_##type##_table[sq];                                       \
         eg[1] += eg_##type##_table[sq];                                       \
      } else {                                                                 \
         mg[0] += mg_##type##_table[sq ^ 56];                                  \
         eg[0] += eg_##type##_table[sq ^ 56];                                  \
      }                                                                        \
   }                                                                           \
   do {                                                                        \
   } while (0)
// based on CPW
// https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function
int pesto_eval(board *brd, int material) {
   int mg[2] = {};
   int eg[2] = {};

   bitboa pawn;
   bitboa bishop;
   bitboa king;
   bitboa queen;
   bitboa rook;
   bitboa knight;

   GET_PIECES(b);
   bool to_play = true;
   LOOP_PESTO(pawn);
   LOOP_PESTO(bishop);
   LOOP_PESTO(king);
   LOOP_PESTO(knight);
   LOOP_PESTO(rook);
   LOOP_PESTO(queen);

   GET_PIECES(w);
   to_play = false;
   LOOP_PESTO(pawn);
   LOOP_PESTO(bishop);
   LOOP_PESTO(king);
   LOOP_PESTO(knight);
   LOOP_PESTO(rook);
   LOOP_PESTO(queen);

   int total_mat = 16 * PAWN_VAL + 4 * BISHOP_VAL + 4 * ROOK_VAL +
                   2 * QUEEN_VAL + 4 * KNIGHT_VAL;
   int score_w = material * mg[0] + (total_mat - material) * eg[0];
   int score_b = material * mg[1] + (total_mat - material) * eg[1];
   return (score_w - score_b) / (24 * PAWN_VAL);
}
#undef GET_PIECES
#undef LOOP_PESTO

int eval(board *brd) {
   int material = material_eval(brd);
   int pesto = pesto_eval(brd, material);

   int score = material + pesto;
   // always get score positive relative to current player,
   // needed for negamax
   return brd->to_play ? -score : score;
}
