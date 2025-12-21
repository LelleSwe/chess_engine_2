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
   /// White en passant.
   /// Overlaps the pawn that can passant.
   bitboa white_enp;
   /// Black en passant.
   /// Overlaps the pawn that can passant.
   bitboa black_enp;
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
   castle_right castle;
   bool to_play;
   bool en_passant;
} move;

#endif

// Generates a default chess board, with pieces.
board gen_start_board();
void from_long_algebraic(char *alg_string, board *_board);
void from_long_alg_single(move *to_move, const board *_board,
                          const char *single_fen, const bool to_play);
bitboa brd_from_pos(const char *pos);
bitboa comb_from_comp(const board *brd, bool to_play);

// Compares two boards for equality.
// only nonzero if all fields of the board struct are equal.
bool board_cmp(const board *a, const board *b);
bool move_cmp(const move *a, const move *b);
piece get_piece(const board *brd, const bitboa position);
void flip_piece(board *brd, const piece pc, const bool to_play,
                const bitboa place);
bool try_castle(board *brd, const move *mov);
