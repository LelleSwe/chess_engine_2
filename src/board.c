#include "board.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

inline board gen_start_board() {
   board board = {0x000000000000FF00UL,
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
   return board;
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

/// Does not check for move legality.
/// Undefined behaviour if trying to capture one's own pieces.
/// Use castle() for castling pieces.
/// Piece:
///   0 - pawn
///   1 - bishop
///   2 - rook
///   3 - queen
///   4 - king
///   5 - knight
/// to_play: 0 for white, 1 for black
void make_move(board *board, move *move) {
   // Sets the corresponding piece.
   ((bitboa *)board)[move->piece + 7 * move->to_play] ^= move->from;
   ((bitboa *)board)[move->piece + 7 * move->to_play] ^= move->to;
   // Sets the combined bitboard->
   ((bitboa *)board)[6 + 7 * move->to_play] ^= move->from;
   ((bitboa *)board)[6 + 7 * move->to_play] ^= move->to;

   // Find out (if any) which piece is occupying next position
   if (((bitboa *)board)[6 + 7 * (1 - move->to_play)] & move->to) {
      for (int i = 0; i < 6; i++) {
         if (((bitboa *)board)[i + 7 * (1 - move->to_play)] & move->to) {
            ((bitboa *)board)[i + 7 * (1 - move->to_play)] ^= move->to;
            ((bitboa *)board)[6 + 7 * (1 - move->to_play)] ^= move->to;
            break;
         }
      }
   }
   // King castling rights
   if (move->piece == 5 && (board->black_castle | board->white_castle)) {
      if (move->to_play) {
         board->black_castle = 0;
      } else {
         board->white_castle = 0;
      }
   }
   // En passant rights
   if (move->to_play) {
      board->black_enp = 0;
   } else {
      board->white_enp = 0;
   }
   // Rook castling rights
   if (move->piece == 2) {
      if (move->to_play == 0) {
         if (board->white_castle && (move->from & board->wro_bb)) {
            if ((move->from & board->wro_bb) == 1) {
               board->white_castle ^= 1;
            } else {
               board->white_castle ^= 2;
            }
         }
      } else {
         if (board->black_castle && (move->from & board->bro_bb)) {
            if ((move->from & board->bro_bb) == 0x1000000000000) {
               board->black_castle ^= 1;
            } else {
               board->black_castle ^= 2;
            }
         }
      }
   }

   return;
}

/// Gets the piece type at the position asked.
/// Position is a bitboard.
/// -1 for none.
/// White:
///   0 - pawn
///   1 - bishop
///   2 - rook
///   3 - queen
///   4 - king
///   5 - knight
/// Black:
///   6 - pawn
///   7 - bishop
///   8 - rook
///   9 - queen
///   10 - king
///   11 - knight
int get_piece(board *board, bitboa position) {
   // detect if no piece
   if (!(position & (board->wcb_bb | board->bcb_bb))) {
      return -1;
   }

   if (position & board->wcb_bb) {
      for (int i = 0; i < 6; i++) {
         if (position & ((bitboa *)board)[i]) {
            return i;
         }
      }
   } else {
      for (int i = 6; i < 12; i++) {
         if (position & ((bitboa *)board)[i + 1]) {
            return i;
         }
      }
   }

   // Something is wrong.
   printf("position: %llu has neither a piece nor no piece on it.", position);
   return -1;
}

void from_long_alg_single(move *to_move, board *_board, char *single_fen,
                          int to_play) {
   int ffile = single_fen[0] - 'a';
   int frank = single_fen[1] - '1';
   bitboa from_board = (1 << (7 - ffile)) * (1 << (frank * 8));
   // bitboa from_board = (1 << ffile) * (1 << (frank * 8));

   int tfile = single_fen[2] - 'a';
   int trank = single_fen[3] - '1';
   bitboa to_board = (1 << (7 - tfile)) * (1 << (trank * 8));

   assert(0 <= ffile && ffile < 8);
   assert(0 <= frank && frank < 8);
   assert(0 <= tfile && tfile < 8);
   assert(0 <= trank && trank < 8);

   int piece = get_piece(_board, from_board);
   to_move->from = from_board;
   to_move->to = to_board;
   to_move->piece = piece;
   to_move->to_play = to_play;
}

#include "debug_io.h"

void from_long_algebraic(char *alg_string, board *_board) {
   memcpy(_board, &default_start_board, sizeof(board));

   int to_play = 0;
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
      to_play ^= 1;

      print_move(&to_make);
      make_move(_board, &to_make);
   }
}
