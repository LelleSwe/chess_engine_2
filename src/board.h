#ifndef BOARD_DEF
#define BOARD_DEF

#include <stdint.h>
// Bitboard
#define bitboa uint64_t

// 768 bits.
// white pawn bitboard.
// white bishop bitboard.
// white rook bitboard.
// white queen bitboard.
// white king bitboard.
// white knight bitboard.
// Combined bitboard of all white pieces
// black pawn bitboard.
// black bishop bitboard.
// black rook bitboard.
// black queen bitboard.
// black king bitboard.
// black knight bitboard.
// Combined bitboard of all black pieces
// The bitboards have the highest set bit in the top left,
// and lowest set in bottom right, traversing horisontally.
typedef struct {
   bitboa wpa_bb;
   bitboa wbi_bb;
   bitboa wro_bb;
   bitboa wqu_bb;
   bitboa wki_bb;
   bitboa wkn_bb;
   // Combined bitboard of all white pieces
   bitboa wcb_bb;
   bitboa bpa_bb;
   bitboa bbi_bb;
   bitboa bro_bb;
   bitboa bqu_bb;
   bitboa bki_bb;
   bitboa bkn_bb;
   // Combined bitboard of all black pieces
   bitboa bcb_bb;
   /// White castling rights.
   /// 0 = no rights.
   /// 1 = short castle.
   /// 2 = long castle.
   /// 3 = both.
   int white_castle;
   /// White castling rights.
   /// 0 = no rights.
   /// 1 = short castle.
   /// 2 = long castle.
   /// 3 = both.
   int black_castle;
   /// White en passant.
   /// Overlaps the pawn allowing.
   bitboa white_enp;
   /// Black en passant.
   /// Overlaps the pawn allowing.
   bitboa black_enp;
} board;

extern const board default_start_board;
#endif

#ifndef MOVE_STRUCT
#define MOVE_STRUCT

/*
from and to are bitboards for the desired piece.
piece is 0 - 5, for pawn - knight
to_play is whose turn it is, white = 0, black = 1
*/
typedef struct {
   bitboa from;
   bitboa to;
   int piece;
   int to_play;
} move;

#endif

// Generates a default chess board, with pieces.
board gen_start_board();
void from_long_algebraic(char *alg_string, board *_board);
// Compares two boards for equality.
// only nonzero if all fields of the board struct are equal.
int board_cmp(board *a, board *b);
