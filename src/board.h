#include "util.h"
#ifndef BOARD_DEF
#define BOARD_DEF

#include <stdint.h>
// Bitboard
#define bitboa uint64_t

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
   castle_right white_castle;
   /// Black castling rights.
   /// 0 = no rights.
   /// 1 = short castle.
   /// 2 = long castle.
   /// 3 = both.
   castle_right black_castle;
   /// En passant.
   /// Overlaps the pawn that can passant.
   bitboa enp;
} board;

extern const board default_start_board;
#endif

#ifndef MOVE_STRUCT
#define MOVE_STRUCT

/*
 * from and to are bitboards for the desired piece.
 * pc is piece being moved.
 * to_play is whose turn it is, white = false, black = true.
 * castle is castling rights for previous position for to_play.
 * capture is what piece is captured.
 */
typedef struct {
   bitboa from;
   bitboa to;
   piece pc;
   piece capture;
   piece promotion;
   castle_right castle;
   bool to_play;
   bitboa en_passant;
   bitboa prev_en_passant;
} move;

DECLARE_VEC(move)
#endif

// Generates a default chess board, with pieces.
board gen_start_board();
void from_long_algebraic(const char *alg_string, board *_board,
                         vec_move *moves);
void from_long_alg_single(move *to_move, const board *_board,
                          const char *single_fen, const bool to_play);
bitboa brd_from_pos(const char *pos);
bitboa comb_from_comp(const board *brd, bool to_play);

bool board_cmp(const board *a, const board *b);
bool move_cmp(const move *a, const move *b);
// the following are internal functions, not to be used.
piece get_piece(const board *brd, const bitboa position);
void flip_piece(board *brd, const piece pc, const bool to_play,
                const bitboa place);
bool try_castle(board *brd, const move *mov);
bool try_promote(board *brd, const move *mov);
void try_capture(board *brd, const move *mov);
bool try_en_passant(board *brd, const move *mov);
void make_move(board *brd, const move *mov);
void try_undo_capture(board *brd, const move *mov);
bool try_undo_castle(board *brd, const move *mov);
bool try_undo_en_passant(board *brd, const move *mov);
bool try_undo_promote(board *brd, const move *mov);
void undo_move(board *brd, const move *mov);
// the main functions for making (push) and undoing (pop) moves.
void push_move(board *brd, vec_move *moves, const move *mov);
void pop_move(board *brd, vec_move *moves);
