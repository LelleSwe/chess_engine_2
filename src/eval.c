#include "eval.h"

int material_eval(board *brd) {
   int white =
       bit_count(brd->wpa_bb) * PAWN_VAL + bit_count(brd->wkn_bb) * KNIGHT_VAL +
       bit_count(brd->wbi_bb) * BISHOP_VAL + bit_count(brd->wki_bb) * KING_VAL +
       bit_count(brd->wqu_bb) * QUEEN_VAL + bit_count(brd->wro_bb) * ROOK_VAL;
   int black =
       bit_count(brd->bpa_bb) * PAWN_VAL + bit_count(brd->bkn_bb) * KNIGHT_VAL +
       bit_count(brd->bbi_bb) * BISHOP_VAL + bit_count(brd->bki_bb) * KING_VAL +
       bit_count(brd->bqu_bb) * QUEEN_VAL + bit_count(brd->bro_bb) * ROOK_VAL;

   return white - black;
}

uint32_t eval(board *brd) {
   int score = material_eval(brd);

   // always get score positive relative to current player,
   // needed for negamax
   return brd->to_play ? -score : score;
}
