#ifndef BOARD_DEF
#define BOARD_DEF

// Bitboard
#define bitboa unsigned long long

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
} board;

extern const board default_start_board;
#endif


board gen_start_board();