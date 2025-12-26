#include "util.h"
#ifndef BOARD_DEF
#define BOARD_DEF

#include <stdint.h>
// Bitboard
#define bitboa uint64_t

#ifndef MOVE_STRUCT
#define MOVE_STRUCT

typedef enum {
   NO_CASTLE = 0,
   SHORT_CASTLE = 1,
   LONG_CASTLE = 2,
   BOTH_CASTLE = 3
} castle_right;
typedef enum {
   NO_PIECE = 0,
   PAWN = 1,
   BISHOP = 2,
   ROOK = 3,
   QUEEN = 4,
   KING = 5,
   KNIGHT = 6,
} piece;

// clang-format off

typedef enum { 
   a1, b1, c1, d1, e1, f1, g1, h1,
   a2, b2, c2, d2, e2, f2, g2, h2,
   a3, b3, c3, d3, e3, f3, g3, h3,
   a4, b4, c4, d4, e4, f4, g4, h4,
   a5, b5, c5, d5, e5, f5, g5, h5,
   a6, b6, c6, d6, e6, f6, g6, h6,
   a7, b7, c7, d7, e7, f7, g7, h7,
   a8, b8, c8, d8, e8, f8, g8, h8,
} square;

// clang-format on

/*
 * bits 0-5 are for from square
 * bits 6-11 are for to_square
 * bits 12-14 are for promotion piece
 */
typedef uint16_t move;

DECLARE_ARR(move, 4096)
DECLARE_ARR(int, 4096)
DECLARE_ARR(castle_right, 4096)
DECLARE_ARR(piece, 4096)
DECLARE_ARR(bitboa, 4096)
#endif
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
// The bitboards follow little endian ordering,
// with a1=1, a8=0x100000000000000, h8=0x8000000000000000
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
   // false for currently white, true black
   bool to_play;
   int ply_count;
   /// En passant.
   /// Overlaps the pawn that can passant.
   arr_bitboa4096 enp_history;
   arr_move4096 move_history;
   arr_castle_right4096 white_castle_history;
   arr_castle_right4096 black_castle_history;
   arr_piece4096 capture_history;
   arr_int4096 move50count;
} board;

#endif

// Generates a default chess board, with pieces.
board gen_start_board();
board gen_empty_board();
// UCI compliant version
void from_long_algebraic(const char *alg_string, board *brd);
void from_fen(const char *alg_string, board *brd);
move from_long_alg_single(const char *single_fen);
bitboa brd_from_pos(const char *pos);
bitboa comb_from_comp(const board *brd, bool to_play);
bitboa from_square(square sq);
square from_move(const move mov);
square to_move(const move mov);
piece promotion_move(const move mov);
move move_fromp(const square from, const square to, const piece promote);
move move_from(const square from, const square to);

bool board_cmp_bb(const board *a, const board *b);
// the following are internal functions, not to be used.
piece get_piece(const board *brd, const bitboa position);
void flip_piece(board *brd, const piece pc, const bool to_play,
                const bitboa place);
bool try_castle(board *brd, const piece pc, const bitboa from, const bitboa to);
bool try_promote(board *brd, const piece promote, const bitboa from,
                 const bitboa to);
void try_capture(board *brd, const bitboa place);
bool try_en_passant(board *brd, const piece pc, const bitboa from,
                    const bitboa to);
void make_move(board *brd, const move mov);
void try_undo_capture(board *brd, const bitboa place, const piece capture);
bool try_undo_castle(board *brd, const bitboa from, const bitboa to,
                     const piece pc);
bool try_undo_en_passant(board *brd, const piece pc, const bitboa from,
                         const bitboa to, const piece capture);
bool try_undo_promote(board *brd, const piece promotion, const bitboa from,
                      const bitboa to, const piece capture);
void undo_move(board *brd, const move mov);
