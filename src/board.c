#include "board.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline board gen_start_board() {
   board board_ = {0x000000000000FF00UL,
                   0x0000000000000024UL,
                   0x0000000000000081UL,
                   0x0000000000000010UL,
                   0x0000000000000008UL,
                   0x0000000000000042UL,
                   0x000000000000FFFFUL,
                   0x00FF000000000000UL,
                   0x2400000000000000UL,
                   0x8100000000000000UL,
                   0x1000000000000000UL,
                   0x0800000000000000UL,
                   0x4200000000000000UL,
                   0xFFFF000000000000UL,
                   3,
                   3,
                   0,
                   0};
   return board_;
}

const board default_start_board = {0x000000000000FF00UL,
                                   0x0000000000000024UL,
                                   0x0000000000000081UL,
                                   0x0000000000000010UL,
                                   0x0000000000000008UL,
                                   0x0000000000000042UL,
                                   0x000000000000FFFFUL,
                                   0x00FF000000000000UL,
                                   0x2400000000000000UL,
                                   0x8100000000000000UL,
                                   0x1000000000000000UL,
                                   0x0800000000000000UL,
                                   0x4200000000000000UL,
                                   0xFFFF000000000000UL,
                                   3,
                                   3,
                                   0,
                                   0};

bool board_cmp(const board *a, const board *b) {
   return (a->wpa_bb == b->wpa_bb) && (a->wbi_bb == b->wbi_bb) &&
          (a->wro_bb == b->wro_bb) && (a->wqu_bb == b->wqu_bb) &&
          (a->wki_bb == b->wki_bb) && (a->wkn_bb == b->wkn_bb) &&
          (a->wcb_bb == b->wcb_bb) && (a->bpa_bb == b->bpa_bb) &&
          (a->bbi_bb == b->bbi_bb) && (a->bro_bb == b->bro_bb) &&
          (a->bqu_bb == b->bqu_bb) && (a->bki_bb == b->bki_bb) &&
          (a->bkn_bb == b->bkn_bb) && (a->bcb_bb == b->bcb_bb) &&
          (a->white_castle == b->white_castle) &&
          (a->black_castle == b->black_castle) &&
          (a->white_enp == b->white_enp) && (a->black_enp == b->black_enp);
}

bool move_cmp(const move *a, const move *b) {
   return (a->to_play == b->to_play) && (a->capture == b->capture) &&
          (a->castle == b->castle) && (a->from == b->from) &&
          (a->pc == b->pc) && (a->to == b->to);
}

bitboa brd_from_pos(const char *pos) {
   int ffile = pos[0] - 'a';
   int frank = pos[1] - '1';
   bitboa brd = (1UL << (7UL - ffile)) * (1UL << (frank * 8UL));
   return brd;
}

bitboa comb_from_comp(const board *brd, bool to_play) {
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

void flip_piece(board *brd, const piece pc, const bool to_play,
                const bitboa place) {
   // to_play = false for white's turn
   if (!(to_play)) {
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
         exit(EXIT_FAILURE);
         break;
      }
   }
}

bool try_castle(board *brd, const move *mov) {
   if (mov->to_play) {
      // short castle black
      if (((mov->castle & SHORT_CASTLE) != 0) && (mov->pc == KING) &&
          (mov->from == brd_from_pos("e8")) &&
          (mov->to == brd_from_pos("g8"))) {
         flip_piece(brd, KING, true, brd_from_pos("g8"));
         flip_piece(brd, KING, true, brd_from_pos("e8"));
         flip_piece(brd, ROOK, true, brd_from_pos("h8"));
         flip_piece(brd, ROOK, true, brd_from_pos("f8"));
         brd->black_castle = NO_CASTLE;
         brd->black_enp = 0;
         return true;
      }
      // long castle black
      if (((mov->castle & LONG_CASTLE) != 0) && (mov->pc == KING) &&
          (mov->from == brd_from_pos("e8")) &&
          (mov->to == brd_from_pos("c8"))) {
         flip_piece(brd, KING, true, brd_from_pos("e8"));
         flip_piece(brd, KING, true, brd_from_pos("c8"));
         flip_piece(brd, ROOK, true, brd_from_pos("a8"));
         flip_piece(brd, ROOK, true, brd_from_pos("d8"));
         brd->black_castle = NO_CASTLE;
         brd->black_enp = 0;
         return true;
      }
   } else {
      // short castle white
      if (((mov->castle & SHORT_CASTLE) != 0) && (mov->pc == KING) &&
          (mov->from == brd_from_pos("e1")) &&
          (mov->to == brd_from_pos("g1"))) {
         flip_piece(brd, KING, false, brd_from_pos("g1"));
         flip_piece(brd, KING, false, brd_from_pos("e1"));
         flip_piece(brd, ROOK, false, brd_from_pos("h1"));
         flip_piece(brd, ROOK, false, brd_from_pos("f1"));
         brd->white_castle = NO_CASTLE;
         brd->white_enp = 0;
         return true;
      }
      // long castle white
      if (((mov->castle & LONG_CASTLE) != 0) && (mov->pc == KING) &&
          (mov->from == brd_from_pos("e1")) &&
          (mov->to == brd_from_pos("c1"))) {
         flip_piece(brd, KING, false, brd_from_pos("e1"));
         flip_piece(brd, KING, false, brd_from_pos("c1"));
         flip_piece(brd, ROOK, false, brd_from_pos("a1"));
         flip_piece(brd, ROOK, false, brd_from_pos("d1"));
         brd->white_castle = NO_CASTLE;
         brd->white_enp = 0;
         return true;
      }
   }
   return false;
}

/// Does not check for move legality.
/// Undefined behaviour if trying to capture one's own pieces.
/// to_play: false for white, true for black
void make_move(board *brd, const move *mov) {
   if (try_castle(brd, mov))
      return;
   if (mov->en_passant) {
      puts("ERROR: en_passant not implemented.");
      exit(EXIT_FAILURE);
      return;
   }

   // move piece
   flip_piece(brd, mov->pc, mov->to_play, mov->from);
   if (mov->to_play) {
      flip_piece(brd, mov->pc, true, mov->to);
      if (brd->wcb_bb & mov->to) {
         piece scrap = get_piece(brd, mov->to);
         flip_piece(brd, scrap, false, mov->to);
      }
   } else {
      flip_piece(brd, mov->pc, false, mov->to);
      if (brd->bcb_bb & mov->to) {
         piece scrap = get_piece(brd, mov->to);
         flip_piece(brd, scrap, true, mov->to);
      }
   }
   // King castling rights
   if (mov->pc == KING && (brd->black_castle | brd->white_castle)) {
      if (mov->to_play) {
         brd->black_castle = 0;
      } else {
         brd->white_castle = 0;
      }
   }
   // En passant rights
   if (mov->to_play) {
      brd->black_enp = 0;
   } else {
      brd->white_enp = 0;
   }
   // Rook castling rights
   if (mov->pc == ROOK) {
      // if white's turn
      if (mov->to_play == false) {
         if ((brd->white_castle & SHORT_CASTLE) &&
             ((mov->from | mov->to) & brd->wro_bb) &&
             ((mov->from & brd->wro_bb) == 1)) {
            brd->white_castle ^= SHORT_CASTLE;
         } else if ((brd->white_castle & LONG_CASTLE) &&
                    ((mov->from | mov->to) & brd->wro_bb) &&
                    ((mov->from & brd->wro_bb) == 0x80)) {
            brd->white_castle ^= LONG_CASTLE;
         }
      } else {
         if ((brd->black_castle & SHORT_CASTLE) &&
             ((mov->from | mov->to) & brd->bro_bb) &&
             ((mov->from & brd->bro_bb) == 0x100000000000000)) {
            brd->black_castle ^= SHORT_CASTLE;
         } else if ((brd->black_castle & LONG_CASTLE) &&
                    ((mov->from | mov->to) & brd->bro_bb) &&
                    ((mov->from & brd->bro_bb) == 0x8000000000000000)) {
            brd->black_castle ^= LONG_CASTLE;
         }
      }
   }
}
// void make_move(board *board_, const move *move_) {
//    // Sets the corresponding piece.
//    ((bitboa *)board_)[move_->piece + 7 * move_->to_play] ^= move_->from;
//    ((bitboa *)board_)[move_->piece + 7 * move_->to_play] ^= move_->to;
//    // Sets the combined bitboard_->
//    ((bitboa *)board_)[6 + 7 * move_->to_play] ^= move_->from;
//    ((bitboa *)board_)[6 + 7 * move_->to_play] ^= move_->to;
//
//    // Find out (if any) which piece is occupying next position
//    if (((bitboa *)board_)[6 + 7 * (1 - move_->to_play)] & move_->to) {
//       for (int i = 0; i < 6; i++) {
//          if (((bitboa *)board_)[i + 7 * (1 - move_->to_play)] & move_->to)
//          {
//             ((bitboa *)board_)[i + 7 * (1 - move_->to_play)] ^= move_->to;
//             ((bitboa *)board_)[6 + 7 * (1 - move_->to_play)] ^= move_->to;
//             break;
//          }
//       }
//    }
//    // King castling rights
//    if (move_->piece == 5 && (board_->black_castle | board_->white_castle))
//    {
//       if (move_->to_play) {
//          board_->black_castle = 0;
//       } else {
//          board_->white_castle = 0;
//       }
//    }
//    // En passant rights
//    if (move_->to_play) {
//       board_->black_enp = 0;
//    } else {
//       board_->white_enp = 0;
//    }
//    // Rook castling rights
//    if (move_->piece == 2) {
//       if (move_->to_play == 0) {
//          if (board_->white_castle && (move_->from & board_->wro_bb)) {
//             if ((move_->from & board_->wro_bb) == 1) {
//                board_->white_castle ^= 1;
//             } else {
//                board_->white_castle ^= 2;
//             }
//          }
//       } else {
//          if (board_->black_castle && (move_->from & board_->bro_bb)) {
//             if ((move_->from & board_->bro_bb) == 0x1000000000000) {
//                board_->black_castle ^= 1;
//             } else {
//                board_->black_castle ^= 2;
//             }
//          }
//       }
//    }
//
//    return;
// }

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
   printf("position: %llu has neither a piece nor no piece on it.", position);
   return -1;
}

void from_long_alg_single(move *to_move, const board *_board,
                          const char *single_fen, const bool to_play) {
   int ffile = single_fen[0] - 'a';
   int frank = single_fen[1] - '1';
   bitboa from_board = (1UL << (7UL - ffile)) * (1UL << (frank * 8UL));

   int tfile = single_fen[2] - 'a';
   int trank = single_fen[3] - '1';
   bitboa to_board = (1UL << (7UL - tfile)) * (1UL << (trank * 8UL));

   // printf("move: \"%s\"\n (to %c%c)", single_fen, single_fen[2],
   // single_fen[3]); printf("from: %llu\n", from_board); printf("to:
   // %llu\n", to_board);

   assert(0 <= ffile && ffile < 8);
   assert(0 <= frank && frank < 8);
   assert(0 <= tfile && tfile < 8);
   assert(0 <= trank && trank < 8);

   piece mov_pc = get_piece(_board, from_board);
   piece cap_pc = get_piece(_board, to_board);
   to_move->from = from_board;
   to_move->to = to_board;
   to_move->pc = mov_pc;
   to_move->capture = cap_pc;
   to_move->to_play = to_play;
   to_move->castle = to_play ? _board->black_castle : _board->white_castle;

   // TODO: placeholders!
   // if e1c1 king, then castle_long
   // if e1g1 king, then castle_short
   to_move->en_passant = false;
}

#include "debug_io.h"

void from_long_algebraic(char *alg_string, board *_board) {
   memcpy(_board, &default_start_board, sizeof(board));

   bool to_play = false;
   int idx = 0;
   int len = strlen(alg_string);
   char alg_move[6] = "";
   while (idx < len) {
      move to_make;
      alg_move[0] = alg_string[idx];
      alg_move[1] = alg_string[idx + 1];

      alg_move[2] = alg_string[idx + 2];
      alg_move[3] = alg_string[idx + 3];
      if (isspace(alg_string[idx + 4])) {
         alg_move[4] = alg_string[idx + 4];
         idx++;
      }
      printf("%s\n", alg_move);
      idx += 4;
      from_long_alg_single(&to_make, _board, alg_move, to_play);
      to_play = !to_play;

      print_move(&to_make);
      make_move(_board, &to_make);
   }
}
