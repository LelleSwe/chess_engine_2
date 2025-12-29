#include "board.h"
#include "debug_io.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
DEFINE_ARR(move, 4096)
DEFINE_ARR(piece, 4096)
DEFINE_ARR(castle_right, 4096)
DEFINE_ARR(bitboa, 4096)
DEFINE_ARR(int, 4096)

inline board gen_start_board() {
   arr_move4096 moves = new_arr_move4096();
   arr_push_move4096(&moves, 0);
   arr_piece4096 captures = new_arr_piece4096();
   arr_push_piece4096(&captures, NO_PIECE);
   arr_castle_right4096 white_castle_history = new_arr_castle_right4096();
   arr_push_castle_right4096(&white_castle_history, BOTH_CASTLE);
   arr_castle_right4096 black_castle_history = new_arr_castle_right4096();
   arr_push_castle_right4096(&black_castle_history, BOTH_CASTLE);
   arr_bitboa4096 enp_history = new_arr_bitboa4096();
   arr_push_bitboa4096(&enp_history, 0);
   arr_int4096 move50count = new_arr_int4096();
   arr_push_int4096(&move50count, 0);

   board brd = {.wpa_bb = 0x000000000000FF00UL,
                .wbi_bb = 0x0000000000000024UL,
                .wro_bb = 0x0000000000000081UL,
                .wqu_bb = 0x0000000000000008UL,
                .wki_bb = 0x0000000000000010UL,
                .wkn_bb = 0x0000000000000042UL,
                .wcb_bb = 0x000000000000FFFFUL,
                .bpa_bb = 0x00FF000000000000UL,
                .bbi_bb = 0x2400000000000000UL,
                .bro_bb = 0x8100000000000000UL,
                .bqu_bb = 0x0800000000000000UL,
                .bki_bb = 0x1000000000000000UL,
                .bkn_bb = 0x4200000000000000UL,
                .bcb_bb = 0xFFFF000000000000UL,
                .enp_history = enp_history,
                .move_history = moves,
                .white_castle_history = white_castle_history,
                .black_castle_history = black_castle_history,
                .capture_history = captures,
                .move50count = move50count};
   return brd;
}

inline board gen_empty_board() {
   arr_move4096 moves = new_arr_move4096();
   arr_push_move4096(&moves, 0);
   arr_piece4096 captures = new_arr_piece4096();
   arr_push_piece4096(&captures, NO_PIECE);
   arr_castle_right4096 white_castle_history = new_arr_castle_right4096();
   arr_push_castle_right4096(&white_castle_history, NO_CASTLE);
   arr_castle_right4096 black_castle_history = new_arr_castle_right4096();
   arr_push_castle_right4096(&black_castle_history, NO_CASTLE);
   arr_bitboa4096 enp_history = new_arr_bitboa4096();
   arr_push_bitboa4096(&enp_history, 0);
   arr_int4096 move50count = new_arr_int4096();
   arr_push_int4096(&move50count, 0);

   board brd = {.wpa_bb = 0x0000000000000000UL,
                .wbi_bb = 0x0000000000000000UL,
                .wro_bb = 0x0000000000000000UL,
                .wqu_bb = 0x0000000000000000UL,
                .wki_bb = 0x0000000000000000UL,
                .wkn_bb = 0x0000000000000000UL,
                .wcb_bb = 0x0000000000000000UL,
                .bpa_bb = 0x0000000000000000UL,
                .bbi_bb = 0x0000000000000000UL,
                .bro_bb = 0x0000000000000000UL,
                .bqu_bb = 0x0000000000000000UL,
                .bki_bb = 0x0000000000000000UL,
                .bkn_bb = 0x0000000000000000UL,
                .bcb_bb = 0x0000000000000000UL,
                .enp_history = enp_history,
                .move_history = moves,
                .white_castle_history = white_castle_history,
                .black_castle_history = black_castle_history,
                .capture_history = captures,
                .move50count = move50count};
   return brd;
}

inline bool board_cmp_bb(const board *a, const board *b) {
   return (a->wpa_bb == b->wpa_bb) && (a->wbi_bb == b->wbi_bb) &&
          (a->wro_bb == b->wro_bb) && (a->wqu_bb == b->wqu_bb) &&
          (a->wki_bb == b->wki_bb) && (a->wkn_bb == b->wkn_bb) &&
          (a->wcb_bb == b->wcb_bb) && (a->bpa_bb == b->bpa_bb) &&
          (a->bbi_bb == b->bbi_bb) && (a->bro_bb == b->bro_bb) &&
          (a->bqu_bb == b->bqu_bb) && (a->bki_bb == b->bki_bb) &&
          (a->bkn_bb == b->bkn_bb) && (a->bcb_bb == b->bcb_bb);
}

inline bool board_cmp_hist(const board *a, const board *b) {
   bool sizes =
       (a->black_castle_history.size == b->black_castle_history.size) &&
       (a->white_castle_history.size == b->white_castle_history.size) &&
       (a->move_history.size == b->move_history.size) &&
       (a->ply_count == b->ply_count) &&
       (a->enp_history.size == b->enp_history.size) &&
       (a->to_play == b->to_play) &&
       (a->capture_history.size == b->capture_history.size) &&
       (a->move50count.size == b->move50count.size) &&
       (a->move_history.size == b->move_history.size);

   if (!sizes)
      return false;

   for (uint32_t i = a->black_castle_history.size;
        i < a->black_castle_history.size; i++) {
      if (!(arr_get_castle_right4096(&a->black_castle_history, i) !=
            (arr_get_castle_right4096(&b->black_castle_history, i)))) {
         return false;
      }
      if (!(arr_get_castle_right4096(&a->white_castle_history, i) !=
            (arr_get_castle_right4096(&b->white_castle_history, i)))) {
         return false;
      }
      if (!(arr_get_bitboa4096(&a->enp_history, i) !=
            (arr_get_bitboa4096(&b->enp_history, i)))) {
         return false;
      }
      if (!(arr_get_move4096(&a->move_history, i) !=
            (arr_get_move4096(&b->move_history, i)))) {
         return false;
      }
      if (!(arr_get_int4096(&a->move50count, i) !=
            (arr_get_int4096(&b->move50count, i)))) {
         return false;
      }
      if (!(arr_get_piece4096(&a->capture_history, i) !=
            (arr_get_piece4096(&b->capture_history, i)))) {
         return false;
      }
   }
   return true;
}

void piece_from_char(piece *pc, bool *to_play, const char inp) {
   switch (inp) {
   case 'p':
      *pc = PAWN;
      *to_play = true;
      break;
   case 'b':
      *pc = BISHOP;
      *to_play = true;
      break;
   case 'r':
      *pc = ROOK;
      *to_play = true;
      break;
   case 'q':
      *pc = QUEEN;
      *to_play = true;
      break;
   case 'k':
      *pc = KING;
      *to_play = true;
      break;
   case 'n':
      *pc = KNIGHT;
      *to_play = true;
      break;
   case 'P':
      *pc = PAWN;
      *to_play = false;
      break;
   case 'B':
      *pc = BISHOP;
      *to_play = false;
      break;
   case 'R':
      *pc = ROOK;
      *to_play = false;
      break;
   case 'Q':
      *pc = QUEEN;
      *to_play = false;
      break;
   case 'K':
      *pc = KING;
      *to_play = false;
      break;
   case 'N':
      *pc = KNIGHT;
      *to_play = false;
      break;
   default:
      printf("ERROR: Tried to convert invalid char to piece. %s:%d\n", __FILE__,
             __LINE__);
      exit(EXIT_FAILURE);
   }
}

inline bitboa brd_from_pos(const char *pos) {
   int ffile = pos[0] - 'a';
   int frank = pos[1] - '1';
   bitboa brd = (1UL << ffile) * (1UL << (frank * 8UL));
   return brd;
}

inline bitboa comb_from_comp(const board *brd, bool to_play) {
   bitboa ret;
   if (to_play) {
      ret = brd->bpa_bb | brd->bbi_bb | brd->bro_bb | brd->bqu_bb |
            brd->bki_bb | brd->bkn_bb;
   } else {
      ret = brd->wpa_bb | brd->wbi_bb | brd->wro_bb | brd->wqu_bb |
            brd->wki_bb | brd->wkn_bb;
   }
   return ret;
}

inline square from_move(const move mov) { return mov & 0b111111; }
inline square to_move(const move mov) { return (mov >> 6) & 0b111111; }
inline piece promotion_move(const move mov) { return (mov >> 12) & 0b111; }
inline move move_from(const square from, const square to) {
   return from | (to << 6);
}
inline move move_fromp(const square from, const square to,
                       const piece promote) {
   return from | (to << 6) | (promote << 12);
}
inline bitboa from_square(square sq) { return (1ULL << sq); }

// hopefully this compiles to something nice :P
void flip_piece(board *brd, const piece pc, const bool to_play,
                const bitboa place) {
   static bool seen_flip_error = false;

   // to_play = false for white's turn
   if (!to_play) {
      brd->wcb_bb ^= place;
      switch (pc) {
      case PAWN:
         brd->wpa_bb ^= place;
         break;
      case BISHOP:
         brd->wbi_bb ^= place;
         break;
      case ROOK:
         brd->wro_bb ^= place;
         break;
      case QUEEN:
         brd->wqu_bb ^= place;
         break;
      case KING:
         brd->wki_bb ^= place;
         break;
      case KNIGHT:
         brd->wkn_bb ^= place;
         break;
      case NO_PIECE:
         // Something has gone very wrong
         puts("ERROR: Attempted to flip non-existant piece for white.");
         if (!seen_flip_error) {
            seen_flip_error = true;
            print_move_history(brd);
         }
         exit(EXIT_FAILURE);
         break;
      }
   } else {
      brd->bcb_bb ^= place;
      switch (pc) {
      case PAWN:
         brd->bpa_bb ^= place;
         break;
      case BISHOP:
         brd->bbi_bb ^= place;
         break;
      case ROOK:
         brd->bro_bb ^= place;
         break;
      case QUEEN:
         brd->bqu_bb ^= place;
         break;
      case KING:
         brd->bki_bb ^= place;
         break;
      case KNIGHT:
         brd->bkn_bb ^= place;
         break;
      case NO_PIECE:
         // Something has gone very wrong

         puts("ERROR: Attempted to flip non-existant piece for black.");
         if (!seen_flip_error) {
            seen_flip_error = true;
            print_move_history(brd);
         }
         exit(EXIT_FAILURE);
         break;
      }
   }
}

inline void try_capture(board *brd, const bitboa place,
                        castle_right *white_castle,
                        castle_right *black_castle) {
   if (brd->to_play) {
      if (brd->wcb_bb & place) {
         if ((place == from_square(a1)) && (*white_castle & LONG_CASTLE))
            *white_castle ^= LONG_CASTLE;
         if ((place == from_square(h1)) && (*white_castle & SHORT_CASTLE))
            *white_castle ^= SHORT_CASTLE;

         flip_piece(brd, get_piece(brd, place), false, place);
         // 50 move draw rule
         arr_set_int4096(&brd->move50count, brd->ply_count, 0);
      }
   } else {
      if (brd->bcb_bb & place) {
         if ((place == from_square(a8)) && (*black_castle & LONG_CASTLE))
            *black_castle ^= LONG_CASTLE;
         if ((place == from_square(h8)) && (*black_castle & SHORT_CASTLE))
            *black_castle ^= SHORT_CASTLE;

         flip_piece(brd, get_piece(brd, place), true, place);
         // 50 move draw rule
         arr_set_int4096(&brd->move50count, brd->ply_count, 0);
      }
   }
}

inline bool try_castle(board *brd, const piece pc, const bitboa from,
                       const bitboa to) {
   // printf("piece %d, from %zu, to %zu", pc, from, to);
   if (brd->to_play) {
      // short castle black
      if (((arr_get_castle_right4096(&brd->black_castle_history,
                                     brd->ply_count - 1) &
            SHORT_CASTLE) != 0) &&
          (pc == KING) && (from == brd_from_pos("e8")) &&
          (to == brd_from_pos("g8"))) {
         flip_piece(brd, KING, true, brd_from_pos("g8"));
         flip_piece(brd, KING, true, brd_from_pos("e8"));
         flip_piece(brd, ROOK, true, brd_from_pos("h8"));
         flip_piece(brd, ROOK, true, brd_from_pos("f8"));
         arr_set_castle_right4096(&brd->black_castle_history, brd->ply_count,
                                  NO_CASTLE);
         arr_set_bitboa4096(&brd->enp_history, brd->ply_count, 0);
         return true;
      }
      // long castle black
      if (((arr_get_castle_right4096(&brd->black_castle_history,
                                     brd->ply_count - 1) &
            LONG_CASTLE) != 0) &&
          (pc == KING) && (from == brd_from_pos("e8")) &&
          (to == brd_from_pos("c8"))) {
         flip_piece(brd, KING, true, brd_from_pos("e8"));
         flip_piece(brd, KING, true, brd_from_pos("c8"));
         flip_piece(brd, ROOK, true, brd_from_pos("a8"));
         flip_piece(brd, ROOK, true, brd_from_pos("d8"));
         arr_set_castle_right4096(&brd->black_castle_history, brd->ply_count,
                                  NO_CASTLE);
         arr_set_bitboa4096(&brd->enp_history, brd->ply_count, 0);
         return true;
      }
   } else {
      // short castle white
      if (((arr_get_castle_right4096(&brd->white_castle_history,
                                     brd->ply_count - 1) &
            SHORT_CASTLE) != 0) &&
          (pc == KING) && (from == brd_from_pos("e1")) &&
          (to == brd_from_pos("g1"))) {
         flip_piece(brd, KING, false, brd_from_pos("g1"));
         flip_piece(brd, KING, false, brd_from_pos("e1"));
         flip_piece(brd, ROOK, false, brd_from_pos("h1"));
         flip_piece(brd, ROOK, false, brd_from_pos("f1"));
         arr_set_castle_right4096(&brd->white_castle_history, brd->ply_count,
                                  NO_CASTLE);
         arr_set_bitboa4096(&brd->enp_history, brd->ply_count, 0);
         return true;
      }
      // long castle white
      if (((arr_get_castle_right4096(&brd->white_castle_history,
                                     brd->ply_count - 1) &
            LONG_CASTLE) != 0) &&
          (pc == KING) && (from == brd_from_pos("e1")) &&
          (to == brd_from_pos("c1"))) {
         flip_piece(brd, KING, false, brd_from_pos("e1"));
         flip_piece(brd, KING, false, brd_from_pos("c1"));
         flip_piece(brd, ROOK, false, brd_from_pos("a1"));
         flip_piece(brd, ROOK, false, brd_from_pos("d1"));
         arr_set_castle_right4096(&brd->white_castle_history, brd->ply_count,
                                  NO_CASTLE);
         arr_set_bitboa4096(&brd->enp_history, brd->ply_count, 0);
         return true;
      }
   }
   return false;
}

inline bool try_promote(board *brd, const piece promote, const bitboa from,
                        const bitboa to, castle_right *white_castle,
                        castle_right *black_castle) {
   if (promote != NO_PIECE) {
      try_capture(brd, to, white_castle, black_castle);
      flip_piece(brd, PAWN, brd->to_play, from);
      flip_piece(brd, promote, brd->to_play, to);
      return true;
   }
   return false;
}

inline bool try_en_passant(board *brd, const piece pc, const bitboa from,
                           const bitboa to) {
   bitboa enp = arr_get_bitboa4096(&brd->enp_history, brd->ply_count - 1);
   if (enp) {
      if ((pc == PAWN) &&
          ((__builtin_clzll(from) % 8) != (__builtin_clzll(to) % 8)) &&
          (get_piece(brd, to) == NO_PIECE)) {
         flip_piece(brd, PAWN, !(brd->to_play), enp);
         flip_piece(brd, PAWN, brd->to_play, from);
         flip_piece(brd, PAWN, brd->to_play, to);
         return true;
      }
   }
   return false;
}

/// Does not check for move legality.
/// Undefined behaviour if trying to capture one's own pieces.
/// to_play: false for white, true for black
void make_move(board *brd, const move mov) {
   // so ply_count affects current move,
   // and ply_count-1 is last move
   brd->ply_count++;
   bitboa to = from_square(to_move(mov));
   bitboa from = from_square(from_move(mov));
   piece promote = promotion_move(mov);
   piece capture = get_piece(brd, to);
   piece pc = get_piece(brd, from);

   arr_push_move4096(&brd->move_history, mov);
   arr_push_bitboa4096(&brd->enp_history, 0);
   castle_right white_castle =
       arr_get_castle_right4096(&brd->white_castle_history, brd->ply_count - 1);
   castle_right black_castle =
       arr_get_castle_right4096(&brd->black_castle_history, brd->ply_count - 1);
   arr_push_castle_right4096(&brd->white_castle_history, white_castle);
   arr_push_castle_right4096(&brd->black_castle_history, black_castle);
   arr_push_piece4096(&brd->capture_history, capture);
   arr_push_int4096(&brd->move50count,
                    arr_get_int4096(&brd->move50count, brd->ply_count - 1) + 1);

   // 50 move draw rule
   if (pc == PAWN)
      arr_set_int4096(&brd->move50count, brd->ply_count, 0);

   if (try_castle(brd, pc, from, to)) {
      brd->to_play = !brd->to_play;
      return;
   }
   if (try_promote(brd, promote, from, to, &white_castle, &black_castle)) {
      brd->to_play = !brd->to_play;
      return;
   }
   if (try_en_passant(brd, pc, from, to)) {
      brd->to_play = !brd->to_play;
      return;
   }

   // move piece
   try_capture(brd, to, &white_castle, &black_castle);
   flip_piece(brd, pc, brd->to_play, from);
   flip_piece(brd, pc, brd->to_play, to);

   // 16 to account for ranks/files
   if ((pc == PAWN) && (((int)(from_move(mov) - to_move(mov)) == -16) ||
                        (from_move(mov) - to_move(mov) == 16))) {
      // printf("detected?\n");
      arr_set_bitboa4096(&brd->enp_history, brd->ply_count, to);
   }

   // King castling rights
   if (pc == KING) {
      if (brd->to_play) {
         black_castle = 0;
      } else {
         white_castle = 0;
      }
   }

   // Rook castling rights
   if (pc == ROOK) {
      // if white's turn
      if (brd->to_play == false) {
         if ((white_castle & SHORT_CASTLE) && ((from | to) & brd->wro_bb) &&
             ((from & brd->wro_bb) == brd_from_pos("a8"))) {
            white_castle ^= SHORT_CASTLE;
         } else if ((white_castle & LONG_CASTLE) &&
                    ((from | to) & brd->wro_bb) &&
                    ((from & brd->wro_bb) == brd_from_pos("a1"))) {
            white_castle ^= LONG_CASTLE;
         }
      } else {
         if ((black_castle & SHORT_CASTLE) && ((from | to) & brd->bro_bb) &&
             ((from & brd->bro_bb) == brd_from_pos("h8"))) {
            black_castle ^= SHORT_CASTLE;
         } else if ((black_castle & LONG_CASTLE) &&
                    ((from | to) & brd->bro_bb) &&
                    ((from & brd->bro_bb) == brd_from_pos("a8"))) {
            black_castle ^= LONG_CASTLE;
         }
      }
   }
   brd->to_play = !brd->to_play;
   arr_set_castle_right4096(&brd->white_castle_history, brd->ply_count,
                            white_castle);
   arr_set_castle_right4096(&brd->black_castle_history, brd->ply_count,
                            black_castle);
}

/// Gets the piece type at the position asked.
/// Position is a bitboard.
piece get_piece(const board *brd, const bitboa position) {
   // detect if no piece
   if ((position & (brd->wcb_bb | brd->bcb_bb)) == 0) {
      return NO_PIECE;
   }

   if (position & brd->wcb_bb) {
      if (position & brd->wpa_bb)
         return PAWN;
      if (position & brd->wbi_bb)
         return BISHOP;
      if (position & brd->wro_bb)
         return ROOK;
      if (position & brd->wqu_bb)
         return QUEEN;
      if (position & brd->wki_bb)
         return KING;
      if (position & brd->wkn_bb)
         return KNIGHT;
   } else {
      if (position & brd->bpa_bb)
         return PAWN;
      if (position & brd->bbi_bb)
         return BISHOP;
      if (position & brd->bro_bb)
         return ROOK;
      if (position & brd->bqu_bb)
         return QUEEN;
      if (position & brd->bki_bb)
         return KING;
      if (position & brd->bkn_bb)
         return KNIGHT;
   }

   // Something is wrong.
   printf("ERROR: position: %zu has neither a piece nor no piece on it.",
          position);
   exit(EXIT_FAILURE);
   return -1;
}

void try_undo_capture(board *brd, const bitboa place, const piece capture) {
   if (capture != NO_PIECE) {
      flip_piece(brd, capture, !(brd->to_play), place);
   }
}

bool try_undo_castle(board *brd, const bitboa from, const bitboa to,
                     const piece pc) {
   bool to_play = brd->to_play;
   if (to_play) {
      castle_right castle = arr_get_castle_right4096(&brd->black_castle_history,
                                                     brd->ply_count - 1);
      // short castle black
      if (((pc == KING) && (castle & SHORT_CASTLE) != 0) &&
          (from == brd_from_pos("e8")) && (to == brd_from_pos("g8"))) {
         flip_piece(brd, KING, true, brd_from_pos("g8"));
         flip_piece(brd, KING, true, brd_from_pos("e8"));
         flip_piece(brd, ROOK, true, brd_from_pos("h8"));
         flip_piece(brd, ROOK, true, brd_from_pos("f8"));
         // printf("varför gör du på detta viset svart kort %d %d %d\n",
         //        mov->castle, brd->white_castle, brd->black_castle);
         return true;
      }
      // long castle black
      if (((pc == KING) && (castle & LONG_CASTLE) != 0) &&
          (from == brd_from_pos("e8")) && (to == brd_from_pos("c8"))) {
         flip_piece(brd, KING, true, brd_from_pos("e8"));
         flip_piece(brd, KING, true, brd_from_pos("c8"));
         flip_piece(brd, ROOK, true, brd_from_pos("a8"));
         flip_piece(brd, ROOK, true, brd_from_pos("d8"));
         // printf("varför gör du på detta viset svart lång %d %d %d\n",
         //        mov->castle, brd->white_castle, brd->black_castle);
         return true;
      }
   } else {
      castle_right castle = arr_get_castle_right4096(&brd->white_castle_history,
                                                     brd->ply_count - 1);
      // short castle white
      if (((pc == KING) && (castle & SHORT_CASTLE) != 0) &&
          (from == brd_from_pos("e1")) && (to == brd_from_pos("g1"))) {
         flip_piece(brd, KING, false, brd_from_pos("g1"));
         flip_piece(brd, KING, false, brd_from_pos("e1"));
         flip_piece(brd, ROOK, false, brd_from_pos("h1"));
         flip_piece(brd, ROOK, false, brd_from_pos("f1"));
         // printf("varför gör du på detta viset vit kort %d %d %d\n",
         // mov->castle,
         //        brd->white_castle, brd->black_castle);
         return true;
      }
      // long castle white
      if (((pc == KING) && (castle & LONG_CASTLE) != 0) &&
          (from == brd_from_pos("e1")) && (to == brd_from_pos("c1"))) {
         flip_piece(brd, KING, false, brd_from_pos("e1"));
         flip_piece(brd, KING, false, brd_from_pos("c1"));
         flip_piece(brd, ROOK, false, brd_from_pos("a1"));
         flip_piece(brd, ROOK, false, brd_from_pos("d1"));
         // printf("varför gör du på detta viset vit lång %d %d %d\n",
         // mov->castle,
         //        brd->white_castle, brd->black_castle);
         return true;
      }
   }
   return false;
}
bool try_undo_en_passant(board *brd, const piece pc, const bitboa from,
                         const bitboa to, const piece capture) {
   if ((pc == PAWN) &&
       ((__builtin_clzll(from) % 8) != (__builtin_clzll(to) % 8)) &&
       (capture == NO_PIECE)) {
      bitboa enp = arr_get_bitboa4096(&brd->enp_history, brd->ply_count - 1);
      flip_piece(brd, PAWN, !(brd->to_play), enp);
      flip_piece(brd, PAWN, brd->to_play, from);
      flip_piece(brd, PAWN, brd->to_play, to);
      return true;
   }
   return false;
}

bool try_undo_promote(board *brd, const piece promotion, const bitboa from,
                      const bitboa to, const piece capture) {
   bool to_play = brd->to_play;
   if (promotion != NO_PIECE) {
      flip_piece(brd, PAWN, to_play, from);
      flip_piece(brd, promotion, to_play, to);
      try_undo_capture(brd, to, capture);
      return true;
   }
   return false;
}
void undo_move(board *brd, const move mov) {
   brd->to_play = !brd->to_play;

   arr_drop_bitboa4096(&brd->enp_history);

   bitboa to = from_square(to_move(mov));
   bitboa from = from_square(from_move(mov));
   piece promote = promotion_move(mov);
   piece capture = arr_pop_piece4096(&brd->capture_history);
   piece pc = get_piece(brd, to);

   arr_drop_move4096(&brd->move_history);
   arr_drop_castle_right4096(&brd->white_castle_history);
   arr_drop_castle_right4096(&brd->black_castle_history);
   arr_drop_int4096(&brd->move50count);

   if (try_undo_castle(brd, from, to, pc)) {
      brd->ply_count--;
      return;
   }
   if (try_undo_promote(brd, promote, from, to, capture)) {
      brd->ply_count--;
      return;
   }
   if (try_undo_en_passant(brd, pc, from, to, capture)) {
      brd->ply_count--;
      return;
   }

   // move piece
   flip_piece(brd, pc, brd->to_play, from);
   flip_piece(brd, pc, brd->to_play, to);
   try_undo_capture(brd, to, capture);
   brd->ply_count--;
}

move from_long_alg_single(const char *single_fen) {
   int ffile = single_fen[0] - 'a';
   int frank = single_fen[1] - '1';
   int from = frank * 8 + ffile;

   int tfile = single_fen[2] - 'a';
   int trank = single_fen[3] - '1';
   int to = trank * 8 + tfile;
   piece promotion;
   if (!isspace(single_fen[4])) {
      switch (single_fen[4]) {
      case 'r':
         promotion = ROOK;
         break;
      case 'q':
         promotion = QUEEN;
         break;
      case 'n':
         promotion = KNIGHT;
         break;
      case 'b':
         promotion = BISHOP;
         break;
      default:
         promotion = NO_PIECE;
         break;
      }
   } else {
      promotion = NO_PIECE;
   }

   assert(ffile < 8);
   assert(frank < 8);
   assert(tfile < 8);
   assert(trank < 8);

   return move_fromp(from, to, promotion);
}

// #include "debug_io.h"

void from_long_algebraic(const char *alg_string, board *brd) {
   *brd = gen_start_board();

   int idx = 0;
   int len = strlen(alg_string);
   while (idx < len) {
      char alg_move[6] = "";
      alg_move[0] = alg_string[idx];
      alg_move[1] = alg_string[idx + 1];

      alg_move[2] = alg_string[idx + 2];
      alg_move[3] = alg_string[idx + 3];
      if (!isspace(alg_string[idx + 4])) {
         alg_move[4] = alg_string[idx + 4];
         idx++;
      }
      // printf("\"%s\"\n", alg_move);
      idx += 5;
      move mov = from_long_alg_single(alg_move);

      // print_move(&to_make);
      // printf("%d ", get_piece(_board, to_make.from));
      make_move(brd, mov);
      // printf("%d\n", get_piece(_board, to_make.to));
      // print_board(_board);
   }
}

// with how fragile this function is, the fen string _better_ be formatted
// correctly.
// Otherwise only God knows what will happen.
void from_fen(const char *alg_string, board *brd) {
   *brd = gen_empty_board();

   int idx = -1;
   // ranks
   // printf("%s\n", alg_string + idx + 1);
   for (int rank = 0; rank < 8; rank++) {
      int file = 0;
      for (; file < 8; file++) {
         idx++;
         assert(alg_string[idx] != '/');
         if (('0' < alg_string[idx]) && (alg_string[idx] < '9')) {
            file += alg_string[idx] - '1';
            continue;
         }

         // flip pieces
         bool to_play;
         piece pc;
         piece_from_char(&pc, &to_play, alg_string[idx]);
         bitboa place = from_square(8 * (7 - rank) + file);
         flip_piece(brd, pc, to_play, place);
         // printf("square = %d\n", 8 * rank + file);
      }
      idx += 1;
      // printf("file = %d\n", file);
      // printf("%s\n\n\n", alg_string + idx);
   }
   idx++;

   // whose turn
   bool to_play = alg_string[idx] == 'b';
   assert((alg_string[idx] == 'b') || (alg_string[idx] == 'w'));
   brd->to_play = to_play;
   // printf("%c\n", alg_string[idx]);
   // printf("%s\n", alg_string + idx);

   // castling rights
   idx += 1;
   int white_castle = 0;
   int black_castle = 0;
   for (int i = 0; i < 5; i++) {
      idx++;
      if ((alg_string[idx] == '-') || (alg_string[idx] == ' ')) {
         if (alg_string[idx] == '-')
            idx++;
         break;
      }
      switch (alg_string[idx]) {
      case 'K':
         white_castle |= 1;
         break;
      case 'Q':
         white_castle |= 2;
         break;
      case 'k':
         black_castle |= 1;
         break;
      case 'q':
         black_castle |= 2;
         break;
      default:
         printf("ERROR: invalid character type, %s:%d", __FILE__, __LINE__);
         break;
      }
   }
   // printf("white_castle = %d\n", white_castle);
   // printf("black_castle = %d\n", black_castle);
   idx++;
   arr_set_castle_right4096(&brd->white_castle_history, brd->ply_count,
                            white_castle);
   arr_set_castle_right4096(&brd->black_castle_history, brd->ply_count,
                            black_castle);

   // printf("%s\n", alg_string + idx);
   // is any square en passantable?
   // printf("%d %d\n", white_castle, black_castle);
   if (alg_string[idx] != '-') {
      int file = alg_string[idx] - 'a';
      int rank = alg_string[idx + 1] - '1';
      assert(0 <= rank && rank < 8);
      assert(0 <= file && file < 8);

      // the enp square points to 1 behind where it goes
      // so we compensate by adding one
      // (instead of subtracting because it's
      // previous player's passant)
      int squar;
      if (to_play) {
         squar = (rank + 1) * 8 + file;
      } else {
         squar = (rank - 1) * 8 + file;
      }
      idx += 3;

      arr_set_bitboa4096(&brd->enp_history, brd->ply_count, from_square(squar));
      // printf("en passant %d\n", square);
   } else {
      idx += 2;
      // printf("no passant\n");
   }

   // how many plies have passed since last pawn/capture move?
   char half_move[3] = "";
   for (int i = 0; i < 3; i++) {
      idx++;
      if (alg_string[idx - 1] == ' ') {
         break;
      }

      if (i == 3) {
         printf("uh oh :P %s:%d\n", __FILE__, __LINE__);
      }
      half_move[i] = alg_string[idx - 1];
   }
   int half_moves = atoi(half_move);
   arr_set_int4096(&brd->move50count, brd->ply_count, half_moves);

   // printf("50-moves: %d\n", half_moves);
   // printf("%s\n", alg_string + idx);

   // what move is it?
   // this is approx ply_count * 2
   // I guess I'm just not going to use this?
   char movesc[10] = "";
   for (int i = 0; i < 10; i++) {
      idx++;
      if (alg_string[idx - 1] == ' ') {
         break;
      }
      movesc[i] = alg_string[idx - 1];
   }

   int moves = atoi(movesc);
   // printf("moves %d\n", moves);
   // printf("movesc? %s\n", movesc);
   // printf("%s\n", alg_string + idx);
}
