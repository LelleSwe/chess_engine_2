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

typedef enum : bitboa {
   rank1 = 0x00000000000000FFULL,
   rank2 = 0x000000000000FF00ULL,
   rank3 = 0x0000000000FF0000ULL,
   rank4 = 0x00000000FF000000ULL,
   rank5 = 0x000000FF00000000ULL,
   rank6 = 0x0000FF0000000000ULL,
   rank7 = 0x00FF000000000000ULL,
   rank8 = 0xFF00000000000000ULL,
} ranks;

typedef enum : bitboa {
   file1 = 0x0101010101010101ULL,
   file2 = 0x0202020202020202ULL,
   file3 = 0x0404040404040404ULL,
   file4 = 0x0808080808080808ULL,
   file5 = 0x1010101010101010ULL,
   file6 = 0x2020202020202020ULL,
   file7 = 0x4040404040404040ULL,
   file8 = 0x8080808080808080ULL,
} files;

constexpr bitboa files_arr[8] = {
   0x0101010101010101ULL,
   0x0202020202020202ULL,
   0x0404040404040404ULL,
   0x0808080808080808ULL,
   0x1010101010101010ULL,
   0x2020202020202020ULL,
   0x4040404040404040ULL,
   0x8080808080808080ULL,
};

// top left to bottom right
typedef enum : bitboa {
   diag1  = 0x0100000000000000ULL,
   diag2  = 0x0201000000000000ULL,
   diag3  = 0x0402010000000000ULL,
   diag4  = 0x0804020100000000ULL,
   diag5  = 0x1008040201000000ULL,
   diag6  = 0x2010080402010000ULL,
   diag7  = 0x4020100804020100ULL,
   diag8  = 0x8040201008040201ULL,
   diag9  = 0x0080402010080402ULL,
   diag10 = 0x0000804020100804ULL,
   diag11 = 0x0000008040201008ULL,
   diag12 = 0x0000000080402010ULL,
   diag13 = 0x0000000000804020ULL,
   diag14 = 0x0000000000008040ULL,
   diag15 = 0x0000000000000080ULL,
} diagonals;
constexpr bitboa diagonals_arr[15] = {
   0x0100000000000000ULL,
   0x0201000000000000ULL,
   0x0402010000000000ULL,
   0x0804020100000000ULL,
   0x1008040201000000ULL,
   0x2010080402010000ULL,
   0x4020100804020100ULL,
   0x8040201008040201ULL,
   0x0080402010080402ULL,
   0x0000804020100804ULL,
   0x0000008040201008ULL,
   0x0000000080402010ULL,
   0x0000000000804020ULL,
   0x0000000000008040ULL,
   0x0000000000000080ULL,
};

// bottom left to bottom right
typedef enum : bitboa {
   adiag1  = 0x0000000000000001ULL,
   adiag2  = 0x0000000000000102ULL,
   adiag3  = 0x0000000000010204ULL,
   adiag4  = 0x0000000001020408ULL,
   adiag5  = 0x0000000102040810ULL,
   adiag6  = 0x0000010204081020ULL,
   adiag7  = 0x0001020408102040ULL,
   adiag8  = 0x0102040810204080ULL,
   adiag9  = 0x0204081020408000ULL,
   adiag10 = 0x0408102040800000ULL,
   adiag11 = 0x0810204080000000ULL,
   adiag12 = 0x1020408000000000ULL,
   adiag13 = 0x2040800000000000ULL,
   adiag14 = 0x4080000000000000ULL,
   adiag15 = 0x8000000000000000ULL,
} anti_diagonals;
constexpr bitboa anti_diagonals_arr[15] = {
   0x0000000000000001ULL,
   0x0000000000000102ULL,
   0x0000000000010204ULL,
   0x0000000001020408ULL,
   0x0000000102040810ULL,
   0x0000010204081020ULL,
   0x0001020408102040ULL,
   0x0102040810204080ULL,
   0x0204081020408000ULL,
   0x0408102040800000ULL,
   0x0810204080000000ULL,
   0x1020408000000000ULL,
   0x2040800000000000ULL,
   0x4080000000000000ULL,
   0x8000000000000000ULL,
};

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

// higher 64 bits for comparison and storing in TT
// lower 64 bits (truncated) for lookup
typedef unsigned _BitInt(128) zobrist;
// typedef uint64_t zobrist;

typedef struct {
   zobrist pieces[2][6][64];
   zobrist enp_file[8];
   zobrist castling[2][4];
   zobrist to_play;
} zobrist_lookup_t;

extern zobrist_lookup_t zobrist_lookup;

typedef struct {
   uint64_t hash;
   move best_move;
   // ibv scoring
   int score;
   int depth;
} tt_entry;

DECLARE_ARR(zobrist, 4096)

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
   // arr_bitboa4096 attackers;
   arr_move4096 move_history;
   arr_castle_right4096 white_castle_history;
   arr_castle_right4096 black_castle_history;
   arr_piece4096 capture_history;
   arr_int4096 move50count;
   arr_zobrist4096 zobrist_history;
} board;

#endif

// Generates a default chess board, with pieces.
board gen_start_board();
board gen_empty_board();
// UCI compliant version
void from_long_algebraic(const char *alg_string, board *brd);
int from_fen(const char *alg_string, board *brd);
move from_long_alg_single(const char *single_fen);
bitboa brd_from_pos(const char *pos);
bitboa comb_from_comp(const board *brd, bool to_play);
bitboa from_square(square sq);
square from_move(const move mov);
square to_move(const move mov);
piece promotion_move(const move mov);
move move_fromp(const square from, const square to, const piece promote);
move move_from(const square from, const square to);
void format_move(char *out, const move mov);
char piece_to_prom(piece pc);

bool board_cmp_bb(const board *a, const board *b);
bool board_cmp_hist(const board *a, const board *b);
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

// for zobrist hashes
void flip_hash(board *brd, square from, square to, piece promote, piece pc,
               piece capture);
void flip_hash_piece(board *brd, bitboa pos, bool to_play, piece pc);
void clear_zobrist(board *brd);
void init_zobrist(board *brd);
void setup_zobrist();

// for transposition table
tt_entry *tt_get(zobrist hash);
bool tt_exists(zobrist hash);
void tt_set(zobrist hash, move best_move, int ibv_score, int depth);
