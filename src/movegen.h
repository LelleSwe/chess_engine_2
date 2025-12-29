#include "board.h"

#ifndef movegen_h
#define movegen_h

// rank-file mapping, so a1=0, b1=1
// indexed by squares
extern bitboa knight_moves[64];
extern bitboa king_moves[64];
// masks for hyperbola_quintessence
// the piece that is asked for has bit turned off in lookup
// so c6 is always off when indexing by c6 in any lookup
extern bitboa adiagonal_lookup[64];
extern bitboa diagonal_lookup[64];
extern bitboa hypquin_file_lookup[64];
// indexed by file first, then occupancy bits.
// See get_hypquin_rank
extern uint_fast32_t hypquin_rank_lookup[8][256];

DECLARE_ARR(move, 256)
#endif // movegen_h

// baller name, makes no sense
bitboa hyperbola_quintessence(const bitboa occupancy, const square sq,
                              const bitboa mask);
// only checks for if king is in check
// bool is_legal(board *brd);
bool in_check(board *brd, bool to_play);
bitboa get_attack_bb(board *brd);
void get_pseudolegal_moves(board *brd, arr_move256 *move_list);
void get_legal_moves(board *brd, arr_move256 *move_list);
uint64_t perft(board *brd, int depth);
uint64_t perft_debug(FILE *f, board *brd, int depth);

bitboa from_move_list(arr_move256 *move_list);

bitboa get_king_move(board *brd, square place);
void add_king_moves(board *brd, arr_move256 *move_list);

bitboa get_knight_move(board *brd, square place);
void add_knight_moves(board *brd, arr_move256 *move_list);

bitboa get_bishop_move(board *brd, const square sq);
void add_bishop_moves(board *brd, arr_move256 *move_list);

bitboa get_rook_move(board *brd, square sq);
void add_rook_moves(board *brd, arr_move256 *move_list);

bitboa get_queen_move(board *brd, const square sq);
void add_queen_moves(board *brd, arr_move256 *move_list);

// run at start of program to generate
// lookup tables
void generate_king_moves();
void generate_knight_moves();
void generate_hypquin_rank_lookup();
void generate_hypquin_file_lookup();
void generate_diagonals();
void generate_tables();

// Internal functions
bitboa shift_up(bitboa inp);
bitboa shift_down(bitboa inp);
square bitscan_msb(bitboa inp);
square bitscan_lsb(bitboa inp);
bitboa bswap(bitboa inp);

// pawn moves, idk
bitboa wget_pawn_push1(board *brd);
bitboa wget_pawn_push2(board *brd);
bitboa bget_pawn_push1(board *brd);
bitboa bget_pawn_push2(board *brd);
bitboa wpawn_capture_west(board *brd);
bitboa wpawn_capture_east(board *brd);
bitboa bpawn_capture_west(board *brd);
bitboa bpawn_capture_east(board *brd);
bitboa wpawn_en_passant_west(bitboa wpa_bb, bitboa enp);
bitboa wpawn_en_passant_east(bitboa wpa_bb, bitboa enp);
bitboa bpawn_en_passant_west(bitboa bpa_bb, bitboa enp);
bitboa bpawn_en_passant_east(bitboa bpa_bb, bitboa enp);
bitboa wpromotion(bitboa wpa_bb, bitboa wcb_bb, bitboa bcb_bb);
bitboa bpromotion(bitboa bpa_bb, bitboa wcb_bb, bitboa bcb_bb);
void add_pawn_moves(board *brd, arr_move256 *move_list);

bitboa get_hypquin_rank(bitboa occ, square pc);
