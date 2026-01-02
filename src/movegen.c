#include "board.h"
#include "debug_io.h"
#include "movegen.h"

DEFINE_ARR(move, 256)

// see header file for comments
bitboa knight_moves[64];
bitboa king_moves[64];
bitboa adiagonal_lookup[64];
bitboa diagonal_lookup[64];
bitboa hypquin_file_lookup[64];
uint_fast32_t hypquin_rank_lookup[8][256];

inline bitboa shift_up(bitboa inp) { return inp << 8; }

inline bitboa shift_down(bitboa inp) { return inp >> 8; }

// undefined behaviour if empty bitboard.
// use lsb unless msb ordering is required (x86 after like, 2017 or smth).
// lsb compiles to a tzcnt instruction
// msb compiles to lzcnt, mov, sub instructions
inline square bitscan_msb(bitboa inp) { return 63 - __builtin_clzll(inp); }
inline square bitscan_lsb(bitboa inp) { return __builtin_ctzll(inp); }

inline bitboa bswap(bitboa inp) { return __builtin_bswap64(inp); }
inline uint32_t bit_count(bitboa inp) { return __builtin_popcount(inp); }

bitboa from_move_list(arr_move256 *move_list) {
   bitboa tot = 0;
   for (uint32_t i = 0; i < move_list->size; i++) {
      move mov = arr_get_move256(move_list, i);
      square to = to_move(mov);
      bitboa tob = from_square(to);
      tot |= tob;
   }
   return tot;
}

// generate diagonals / anti diagonals
void generate_diagonals() {
   for (int rank = 0; rank < 8; rank++) {
      for (int file = 0; file < 8; file++) {
         square sq = 8 * rank + file;
         diagonal_lookup[sq] = diagonals_arr[7 - rank + file] ^ from_square(sq);
         adiagonal_lookup[sq] =
             anti_diagonals_arr[rank + file] ^ from_square(sq);
      }
   }
}

void generate_hypquin_file_lookup() {
   for (int rank = 0; rank < 8; rank++) {
      for (int file = 0; file < 8; file++) {
         square sq = 8 * rank + file;
         hypquin_file_lookup[sq] = files_arr[file] ^ from_square(sq);
      }
   }
}

// kings
//
void generate_king_moves() {
   for (square sq = 0; sq < 64; sq++) {
      int rank = sq / 8;
      int file = sq % 8;
      bitboa brd = 0;

      for (int dx = -1; dx < 2; dx++) {
         for (int dy = -1; dy < 2; dy++) {
            if (rank + dy > 7 || rank + dy < 0 || file + dx > 7 ||
                file + dx < 0)
               continue;

            brd |= 1ULL << (8ULL * (rank + dy) + file + dx);
         }
      }

      king_moves[sq] = brd;
   }
}

// assumes that castle rights means king/rooks are in good place
// does not check for checkers
bitboa get_castle_move(board *brd) {
   bitboa shortc = 0;
   bitboa longc = 0;
   if (brd->to_play) {
      castle_right castle =
          arr_get_castle_right4096(&brd->black_castle_history, brd->ply_count);
      int has_right_s = castle & SHORT_CASTLE;
      bool has_space_s = ((brd->bcb_bb | brd->wcb_bb) &
                          (from_square(f8) | from_square(g8))) == 0;
      int has_right_l = castle & LONG_CASTLE;
      bool has_space_l =
          ((brd->bcb_bb | brd->wcb_bb) &
           (from_square(b8) | from_square(c8) | from_square(d8))) == 0;

      if (has_right_s && has_space_s)
         shortc = from_square(g8);
      if (has_right_l && has_space_l)
         longc = from_square(c8);
   } else {
      castle_right castle =
          arr_get_castle_right4096(&brd->white_castle_history, brd->ply_count);
      int has_right_s = castle & SHORT_CASTLE;
      bool has_space_s = ((brd->bcb_bb | brd->wcb_bb) &
                          (from_square(f1) | from_square(g1))) == 0;
      int has_right_l = castle & LONG_CASTLE;
      bool has_space_l =
          ((brd->bcb_bb | brd->wcb_bb) &
           (from_square(b1) | from_square(c1) | from_square(d1))) == 0;

      if (has_right_s && has_space_s)
         shortc = from_square(g1);
      if (has_right_l && has_space_l)
         longc = from_square(c1);
   }

   return shortc | longc;
}

// does not check for king checks
bitboa get_king_move(board *brd, square place) {
   bitboa ret = king_moves[place];
   ret |= get_castle_move(brd);

   if (brd->to_play) {
      ret &= ~brd->bcb_bb;
   } else {
      ret &= ~brd->wcb_bb;
   }
   return ret;
}

// Assumes exactly one king of either colour exists on the board
// Does not check for checkers
void add_king_moves(board *brd, arr_move256 *move_list) {
   square king_pos;
   if (brd->to_play) {
      king_pos = bitscan_lsb(brd->bki_bb);
   } else {
      king_pos = bitscan_lsb(brd->wki_bb);
   }
   bitboa king_bb = get_king_move(brd, king_pos);
   while (king_bb != 0) {
      square to = bitscan_lsb(king_bb);
      move mov = move_from(king_pos, to);
      arr_push_move256(move_list, mov);
      king_bb ^= from_square(to);
   }
}

// knights

void generate_knight_moves() {
   // looping clockwise starting top left
   bitboa rank_offsets[] = {1, 2, 2, 1, -1, -2, -2, -1};
   bitboa file_offsets[] = {-2, -1, 1, 2, 2, 1, -1, -2};
   for (square sq = 0; sq < 64; sq++) {
      bitboa rank = sq / 8;
      bitboa file = sq % 8;
      bitboa brd = 0;

      for (int i = 0; i < 8; i++) {
         bitboa ro = rank_offsets[i];
         bitboa fo = file_offsets[i];

         if (rank + ro > 7 || rank + ro < 0 || file + fo > 7 || file + fo < 0)
            continue;

         brd |= 1ULL << (8ULL * (rank + ro) + file + fo);
      }

      knight_moves[sq] = brd;
   }
}

// does not check for king checks
bitboa get_knight_move(board *brd, square place) {
   bitboa ret = knight_moves[place];
   if (brd->to_play) {
      ret &= ~brd->bcb_bb;
   } else {
      ret &= ~brd->wcb_bb;
   }
   return ret;
}

void add_knight_moves(board *brd, arr_move256 *move_list) {
   bitboa from_bb;
   if (brd->to_play) {
      if (brd->bkn_bb == 0)
         return;
      from_bb = brd->bkn_bb;
   } else {
      if (brd->wkn_bb == 0)
         return;
      from_bb = brd->wkn_bb;
   }

   while (from_bb != 0) {
      square knight_pos = bitscan_lsb(from_bb);
      bitboa knight_bb = get_knight_move(brd, knight_pos);
      while (knight_bb != 0) {
         square to = bitscan_lsb(knight_bb);
         move mov = move_from(knight_pos, to);
         arr_push_move256(move_list, mov);
         knight_bb ^= from_square(to);
      }
      from_bb ^= from_square(knight_pos);
   }
}

// pawns

inline bitboa wget_pawn_push1(board *brd) {
   bitboa ret = brd->wpa_bb;
   ret = shift_up(ret) & ~(brd->bcb_bb | brd->wcb_bb);
   return ret;
}

inline bitboa wget_pawn_push2(board *brd) {
   bitboa ret = brd->wpa_bb;
   ret = shift_up(ret & rank2) & ~(brd->bcb_bb | brd->wcb_bb);
   ret = shift_up(ret) & ~(brd->bcb_bb | brd->wcb_bb);
   return ret;
}

inline bitboa bget_pawn_push1(board *brd) {
   bitboa ret = brd->bpa_bb;
   ret = shift_down(ret) & ~(brd->bcb_bb | brd->wcb_bb);
   return ret;
}

inline bitboa bget_pawn_push2(board *brd) {
   bitboa ret = brd->bpa_bb;
   ret = shift_down(ret & rank7) & ~(brd->bcb_bb | brd->wcb_bb);
   ret = shift_down(ret) & ~(brd->bcb_bb | brd->wcb_bb);
   return ret;
}

inline bitboa wpawn_capture_west(board *brd) {
   bitboa ret = brd->wpa_bb;
   ret = ret & (~file1);
   ret = (ret << 7) & brd->bcb_bb;
   return ret;
}

inline bitboa wpawn_attack_west(board *brd) {
   bitboa ret = brd->wpa_bb;
   ret = ret & (~file1);
   ret = (ret << 7);
   return ret;
}

inline bitboa wpawn_capture_east(board *brd) {
   bitboa ret = brd->wpa_bb;
   ret = ret & (~file8);
   ret = (ret << 9) & brd->bcb_bb;
   return ret;
}

inline bitboa wpawn_attack_east(board *brd) {
   bitboa ret = brd->wpa_bb;
   ret = ret & (~file8);
   ret = (ret << 9);
   return ret;
}

inline bitboa bpawn_capture_west(board *brd) {
   bitboa ret = brd->bpa_bb;
   ret = ret & (~file1);
   ret = (ret >> 9) & brd->wcb_bb;
   return ret;
}

inline bitboa bpawn_attack_west(board *brd) {
   bitboa ret = brd->bpa_bb;
   ret = ret & (~file1);
   ret = (ret >> 9);
   return ret;
}

inline bitboa bpawn_capture_east(board *brd) {
   bitboa ret = brd->bpa_bb;
   ret = ret & (~file8);
   ret = (ret >> 7) & brd->wcb_bb;
   return ret;
}

inline bitboa bpawn_attack_east(board *brd) {
   bitboa ret = brd->bpa_bb;
   ret = ret & (~file8);
   ret = (ret >> 7);
   return ret;
}

inline bitboa wpawn_en_passant_west(bitboa wpa_bb, bitboa enp) {
   bitboa ret = wpa_bb;
   bitboa retw = ret & (~file1);
   retw = (retw << 1) & enp; // detect en passant.
   ret = retw << 8;
   return ret;
}

inline bitboa wpawn_en_passant_east(bitboa wpa_bb, bitboa enp) {
   bitboa ret = wpa_bb;
   bitboa rete = ret & (~file8);
   rete = (rete >> 1) & enp; // detect en passant.
   ret = rete << 8;
   return ret;
}

inline bitboa bpawn_en_passant_west(bitboa bpa_bb, bitboa enp) {
   bitboa ret = bpa_bb;
   bitboa retw = ret & (~file1);
   retw = (retw << 1) & enp; // detect en passant.
   ret = retw >> 8;
   return ret;
}

inline bitboa bpawn_en_passant_east(bitboa bpa_bb, bitboa enp) {
   bitboa ret = bpa_bb;
   bitboa rete = ret & (~file8);
   rete = (rete >> 1) & enp; // detect en passant.

   ret = rete >> 8;
   return ret;
}

inline void promote_push(arr_move256 *move_list, square pawn_pos, square to) {
   move mov;
   mov = move_fromp(pawn_pos, to, QUEEN);
   arr_push_move256(move_list, mov);
   mov = move_fromp(pawn_pos, to, KNIGHT);
   arr_push_move256(move_list, mov);
   mov = move_fromp(pawn_pos, to, ROOK);
   arr_push_move256(move_list, mov);
   mov = move_fromp(pawn_pos, to, BISHOP);
   arr_push_move256(move_list, mov);
}

void add_pawn_moves(board *brd, arr_move256 *move_list) {
   bitboa from_bb;
   if (brd->to_play) {
      if (brd->bpa_bb == 0)
         return;
      from_bb = brd->bpa_bb;
   } else {
      if (brd->wpa_bb == 0)
         return;
      from_bb = brd->wpa_bb;
   }
   bitboa enp = arr_get_bitboa4096(&brd->enp_history, brd->ply_count);

   if (brd->to_play) {

      // pawn pushes
      bitboa push1 = bget_pawn_push1(brd);
      bitboa push2 = bget_pawn_push2(brd);

      // pawn west captures
      bitboa west_cap = bpawn_capture_west(brd);

      // pawn east captures
      bitboa east_cap = bpawn_capture_east(brd);

      while (from_bb != 0) {
         square pawn_pos = bitscan_lsb(from_bb);
         bitboa pawn_bb = from_square(pawn_pos);
         from_bb ^= pawn_bb;

         // captures
         if (((pawn_bb & ~file1) >> 9) & west_cap) {
            square to = bitscan_lsb(pawn_bb >> 9);
            if (to / 8 == 0) {
               promote_push(move_list, pawn_pos, to);

            } else {
               move mov = move_from(pawn_pos, to);
               arr_push_move256(move_list, mov);
            }
         }
         if (((pawn_bb & ~file8) >> 7) & east_cap) {
            square to = bitscan_lsb(pawn_bb >> 7);
            if (to / 8 == 0) {
               promote_push(move_list, pawn_pos, to);
            } else {
               move mov = move_from(pawn_pos, to);
               arr_push_move256(move_list, mov);
            }
         }
         // pushes
         if ((pawn_bb >> 8) & push1) {
            square to = bitscan_lsb(pawn_bb >> 8);
            if (to / 8 == 0) {
               promote_push(move_list, pawn_pos, to);
            } else {
               move mov = move_from(pawn_pos, to);
               arr_push_move256(move_list, mov);
            }
         }
         if ((pawn_bb >> 16) & push2) {
            square to = bitscan_lsb(pawn_bb >> 16);
            move mov = move_from(pawn_pos, to);
            arr_push_move256(move_list, mov);
         }
         // en passant
         // west
         if (((pawn_bb & ~file1) >> 1) & enp) {
            // print_bitboard(pawn_bb);
            square to = bitscan_lsb(pawn_bb >> 9);
            move mov = move_from(pawn_pos, to);
            arr_push_move256(move_list, mov);
         }
         // east
         if (((pawn_bb & ~file8) << 1) & enp) {
            square to = bitscan_lsb(pawn_bb >> 7);
            move mov = move_from(pawn_pos, to);
            arr_push_move256(move_list, mov);
         }
      }
   } else {
      // pawn pushes
      bitboa push1 = wget_pawn_push1(brd);
      bitboa push2 = wget_pawn_push2(brd);

      // pawn west captures
      bitboa west_cap = wpawn_capture_west(brd);

      // pawn east captures
      bitboa east_cap = wpawn_capture_east(brd);

      while (from_bb != 0) {
         square pawn_pos = bitscan_lsb(from_bb);
         bitboa pawn_bb = from_square(pawn_pos);
         from_bb ^= pawn_bb;
         int rank = pawn_pos / 8;
         int file = pawn_pos % 8;

         // captures
         if (((pawn_bb & ~file1) << 7) & west_cap) {
            square to = bitscan_lsb(pawn_bb << 7);
            if (to / 8 == 7) {
               promote_push(move_list, pawn_pos, to);
            } else {
               move mov = move_from(pawn_pos, to);
               arr_push_move256(move_list, mov);
            }
         }
         if (((pawn_bb & ~file8) << 9) & east_cap) {
            square to = bitscan_lsb(pawn_bb << 9);
            if (to / 8 == 7) {
               promote_push(move_list, pawn_pos, to);
            } else {
               move mov = move_from(pawn_pos, to);
               arr_push_move256(move_list, mov);
            }
         }
         // pushes
         if ((pawn_bb << 8) & push1) {
            square to = bitscan_lsb(pawn_bb << 8);
            if (to / 8 == 7) {
               promote_push(move_list, pawn_pos, to);
            } else {
               move mov = move_from(pawn_pos, to);
               arr_push_move256(move_list, mov);
            }
         }
         if ((pawn_bb << 16) & push2) {
            square to = bitscan_lsb(pawn_bb << 16);
            move mov = move_from(pawn_pos, to);
            arr_push_move256(move_list, mov);
         }
         // en passant
         // west
         if (((pawn_bb & ~file1) >> 1) & enp) {
            square to = bitscan_lsb(pawn_bb << 7);
            move mov = move_from(pawn_pos, to);
            arr_push_move256(move_list, mov);
         }
         // east
         if (((pawn_bb & ~file8) << 1) & enp) {
            square to = bitscan_lsb(pawn_bb << 9);
            move mov = move_from(pawn_pos, to);
            arr_push_move256(move_list, mov);
         }
      }
   }
}

// see chess programming wiki c impl lol?
bitboa hyperbola_quintessence(const bitboa occupancy, const square sq,
                              const bitboa mask) {
   bitboa forward, reverse;
   forward = occupancy & mask;
   reverse = bswap(forward);
   forward -= from_square(sq);
   reverse -= bswap(from_square(sq));
   forward ^= bswap(reverse);
   forward &= mask;

   return forward;
}

// bishops

bitboa get_bishop_move(board *brd, const square sq) {
   bitboa occ = brd->wcb_bb | brd->bcb_bb;
   bitboa diagonal = hyperbola_quintessence(occ, sq, diagonal_lookup[sq]);
   bitboa adiagonal = hyperbola_quintessence(occ, sq, adiagonal_lookup[sq]);

   bitboa ret = diagonal + adiagonal;
   if (brd->to_play) {
      ret &= ~(brd->bcb_bb);
   } else {
      ret &= ~(brd->wcb_bb);
   }

   return ret;
}

void add_bishop_moves(board *brd, arr_move256 *move_list) {
   bitboa from_bb;
   if (brd->to_play) {
      if (brd->bbi_bb == 0)
         return;
      from_bb = brd->bbi_bb;
   } else {
      if (brd->wbi_bb == 0)
         return;
      from_bb = brd->wbi_bb;
   }

   while (from_bb != 0) {
      square bishop_pos = bitscan_lsb(from_bb);
      bitboa bishop_bb = get_bishop_move(brd, bishop_pos);
      while (bishop_bb != 0) {
         square to = bitscan_lsb(bishop_bb);
         move mov = move_from(bishop_pos, to);
         arr_push_move256(move_list, mov);
         bishop_bb ^= from_square(to);
      }
      from_bb ^= from_square(bishop_pos);
   }
}

// rooks
// generator for lookup table for hyperbola quintessence ranks
void generate_hypquin_rank_lookup() {
   for (square file = 0; file < 8; file++) {
      for (int occ = 0; occ < 256; occ++) {
         bitboa attack = 0;
         for (int rpoint = file + 1; rpoint < 8; rpoint++) {
            int shifted = (1 << rpoint);

            attack |= shifted;
            if (shifted & occ)
               break;
         }
         for (int lpoint = file - 1; lpoint >= 0; lpoint--) {
            int shifted = (1 << lpoint);

            attack |= shifted;
            if (shifted & occ)
               break;
         }

         // remove attacker from moveable locations
         attack ^= attack & (1 << file);
         hypquin_rank_lookup[file][occ] = attack;
      }
   }
}

// lookup table for hyperbola quintessence ranks
// for rooks
//
// clang-format off
// 
// 11111111    11111111
// 11111111    11111111
// 11111111    11111111
// 11111111    11111111
// 11111111    00101100 <- rank
// 11111111    11111111
// 11111111    11111111
// 11111111    11111111
// all         occ
//
// 00000000    00000000
// 00000000    00000000
// 00000000    00000000
// 00000000    00000000
// 11111111    00101100 <- rank
// 11111111    11111111
// 11111111    11111111
// 11111111    11111111
// shifted     and_occ
//
// 00000000
// 00000000
// 00000000
// 00000000
// 00000000
// 00000000
// 00000000
// 00101100 <- good for lookup!
// shift_occ
//
// then we shift back afterwards
// so we get the right rank
//
// clang-format on
bitboa get_hypquin_rank(bitboa occ, square pc) {
   uint_fast32_t file = pc % 8;
   uint_fast32_t rank = pc / 8;
   bitboa all = (bitboa)(-1);
   bitboa shifted_all = all >> (8 * (7 - rank));
   bitboa and_occ = occ & shifted_all;
   bitboa shift_occ = and_occ >> (8 * rank);
   bitboa looked = hypquin_rank_lookup[file][shift_occ];
   bitboa ret = looked << (8 * rank);
   return ret;
}

bitboa get_rook_move(board *brd, square sq) {
   bitboa occ = brd->wcb_bb | brd->bcb_bb;
   bitboa file = hyperbola_quintessence(occ, sq, hypquin_file_lookup[sq]);
   bitboa rank = get_hypquin_rank(occ, sq);

   bitboa ret = file + rank;
   if (brd->to_play) {
      ret &= ~(brd->bcb_bb);
   } else {
      ret &= ~(brd->wcb_bb);
   }

   return ret;
}

void add_rook_moves(board *brd, arr_move256 *move_list) {
   bitboa from_bb;
   if (brd->to_play) {
      if (brd->bro_bb == 0)
         return;
      from_bb = brd->bro_bb;
   } else {
      if (brd->wro_bb == 0)
         return;
      from_bb = brd->wro_bb;
   }

   while (from_bb != 0) {
      square rook_pos = bitscan_lsb(from_bb);
      bitboa rook_bb = get_rook_move(brd, rook_pos);
      while (rook_bb != 0) {
         square to = bitscan_lsb(rook_bb);
         move mov = move_from(rook_pos, to);
         arr_push_move256(move_list, mov);
         rook_bb ^= from_square(to);
      }
      from_bb ^= from_square(rook_pos);
   }
}

// queens

bitboa get_queen_move(board *brd, const square sq) {
   bitboa occ = brd->wcb_bb | brd->bcb_bb;
   bitboa file = hyperbola_quintessence(occ, sq, hypquin_file_lookup[sq]);
   bitboa rank = get_hypquin_rank(occ, sq);
   bitboa diagonal = hyperbola_quintessence(occ, sq, diagonal_lookup[sq]);
   bitboa adiagonal = hyperbola_quintessence(occ, sq, adiagonal_lookup[sq]);

   bitboa ret = file + rank + diagonal + adiagonal;
   if (brd->to_play) {
      ret &= ~(brd->bcb_bb);
   } else {
      ret &= ~(brd->wcb_bb);
   }

   return ret;
}

void add_queen_moves(board *brd, arr_move256 *move_list) {
   bitboa from_bb;
   if (brd->to_play) {
      if (brd->bqu_bb == 0)
         return;
      from_bb = brd->bqu_bb;
   } else {
      if (brd->wqu_bb == 0)
         return;
      from_bb = brd->wqu_bb;
   }

   while (from_bb != 0) {
      square queen_pos = bitscan_lsb(from_bb);
      bitboa queen_bb = get_queen_move(brd, queen_pos);
      while (queen_bb != 0) {
         square to = bitscan_lsb(queen_bb);
         move mov = move_from(queen_pos, to);
         arr_push_move256(move_list, mov);
         queen_bb ^= from_square(to);
      }
      from_bb ^= from_square(queen_pos);
   }
}

bitboa get_attack_bb(board *brd) {
   bitboa tot = 0;
   // pawns :weary:
   do {
      bitboa enp = arr_get_bitboa4096(&brd->enp_history, brd->ply_count);
      if (brd->to_play) {
         if (brd->bpa_bb == 0)
            break;
         bitboa caps = bpawn_attack_east(brd) | bpawn_attack_west(brd) |
                       bpawn_en_passant_east(brd->bpa_bb, enp) |
                       bpawn_en_passant_west(brd->bpa_bb, enp);
         tot |= caps;
      } else {
         if (brd->wpa_bb == 0)
            break;
         bitboa caps = wpawn_attack_east(brd) | wpawn_attack_west(brd) |
                       wpawn_en_passant_east(brd->bpa_bb, enp) |
                       wpawn_en_passant_west(brd->bpa_bb, enp);
         tot |= caps;
      }
   } while (0);

   // rooks
   do {
      bitboa from_bb;
      if (brd->to_play) {
         if (brd->bro_bb == 0)
            break;
         from_bb = brd->bro_bb;
      } else {
         if (brd->wro_bb == 0)
            break;
         from_bb = brd->wro_bb;
      }

      while (from_bb != 0) {
         square rook_pos = bitscan_lsb(from_bb);
         from_bb ^= from_square(rook_pos);
         bitboa rook_bb = get_rook_move(brd, rook_pos);
         tot |= rook_bb;
      }
   } while (0);

   // bishops
   do {
      bitboa from_bb;
      if (brd->to_play) {
         if (brd->bbi_bb == 0)
            break;
         from_bb = brd->bbi_bb;
      } else {
         if (brd->wbi_bb == 0)
            break;
         from_bb = brd->wbi_bb;
      }
      while (from_bb != 0) {
         square bishop_pos = bitscan_lsb(from_bb);
         from_bb ^= from_square(bishop_pos);
         bitboa bishop_bb = get_bishop_move(brd, bishop_pos);
         tot |= bishop_bb;
      }
   } while (0);

   // kings (?)
   do {
      bitboa from_bb;
      if (brd->to_play) {
         if (brd->bki_bb == 0)
            break;
         from_bb = brd->bki_bb;
      } else {
         if (brd->wki_bb == 0)
            break;
         from_bb = brd->wki_bb;
      }
      square king_pos = bitscan_lsb(from_bb);
      from_bb ^= from_square(king_pos);
      bitboa king_bb = get_king_move(brd, king_pos);
      tot |= king_bb;
   } while (0);

   // queens
   do {
      bitboa from_bb;
      if (brd->to_play) {
         if (brd->bqu_bb == 0)
            break;
         from_bb = brd->bqu_bb;
      } else {
         if (brd->wqu_bb == 0)
            break;
         from_bb = brd->wqu_bb;
      }
      while (from_bb != 0) {
         square queen_pos = bitscan_lsb(from_bb);
         from_bb ^= from_square(queen_pos);
         bitboa queen_bb = get_queen_move(brd, queen_pos);
         tot |= queen_bb;
      }
   } while (0);

   // knights
   do {
      bitboa from_bb;
      if (brd->to_play) {
         if (brd->bkn_bb == 0)
            break;
         from_bb = brd->bkn_bb;
      } else {
         if (brd->wkn_bb == 0)
            break;
         from_bb = brd->wkn_bb;
      }
      while (from_bb != 0) {
         square knight_pos = bitscan_lsb(from_bb);
         from_bb ^= from_square(knight_pos);
         bitboa knight_bb = get_knight_move(brd, knight_pos);
         tot |= knight_bb;
      }
   } while (0);

   return tot;
}

// if previous was castle,
// and king movement overlaps attack bitboard,
// then not legal
bool white_illegal_castle(board *brd, bitboa attacks) {
   move mov = arr_get_move4096(&brd->move_history, brd->ply_count);
   if ((from_move(mov) == e1) && (to_move(mov) == g1) &&
       (get_piece(brd, from_square(g1)) == KING) &&
       (attacks & (from_square(e1) | from_square(f1) | from_square(g1))))
      return true;
   if ((from_move(mov) == e1) && (to_move(mov) == c1) &&
       (get_piece(brd, from_square(c1)) == KING) &&
       (attacks & (from_square(e1) | from_square(d1) | from_square(c1))))
      return true;

   return false;
}

// if previous was castle,
// and king movement overlaps attack bitboard,
// then not legal
bool black_illegal_castle(board *brd, bitboa attacks) {
   move mov = arr_get_move4096(&brd->move_history, brd->ply_count);
   if ((from_move(mov) == e8) && (to_move(mov) == g8) &&
       (get_piece(brd, from_square(g8)) == KING) &&
       (attacks & (from_square(e8) | from_square(f8) | from_square(g8))))
      return true;
   if ((from_move(mov) == e8) && (to_move(mov) == c8) &&
       (get_piece(brd, from_square(c8)) == KING) &&
       (attacks & (from_square(e8) | from_square(d8) | from_square(c8))))
      return true;
   return false;
}

bool prev_wasnt_legal(board *brd, bool to_play) {
   bool prev = brd->to_play;
   brd->to_play = !to_play;
   bitboa attacks = get_attack_bb(brd);
   brd->to_play = prev;

   if (to_play) {
      if (black_illegal_castle(brd, attacks))
         return true;

      return attacks & brd->bki_bb;
   } else {
      if (white_illegal_castle(brd, attacks))
         return true;

      return attacks & brd->wki_bb;
   }
}

bool in_check(board *brd, bool to_play) {
   bool prev = brd->to_play;
   brd->to_play = !to_play;
   bitboa attacks = get_attack_bb(brd);
   brd->to_play = prev;

   if (to_play) {
      return attacks & brd->bki_bb;
   } else {
      return attacks & brd->wki_bb;
   }
}

// doesn't check for king safety
void get_pseudolegal_moves(board *brd, arr_move256 *move_list) {
   add_pawn_moves(brd, move_list);
   add_queen_moves(brd, move_list);
   add_rook_moves(brd, move_list);
   add_knight_moves(brd, move_list);
   add_bishop_moves(brd, move_list);
   add_king_moves(brd, move_list);
}

// pseudolegal, but filters for when king in check
void get_legal_moves(board *brd, arr_move256 *move_list) {
   arr_move256 pseudos = new_arr_move256();
   get_pseudolegal_moves(brd, &pseudos);

   for (uint32_t i = 0; i < pseudos.size; i++) {
      move mov = arr_get_move256(&pseudos, i);
      make_move(brd, mov);
      if (!prev_wasnt_legal(brd, !brd->to_play))
         arr_push_move256(move_list, mov);
      undo_move(brd, mov);
   }
}

// collection of all generators.
void generate_tables() {
   generate_knight_moves();
   generate_king_moves();
   generate_hypquin_rank_lookup();
   generate_hypquin_file_lookup();
   generate_diagonals();
}

// #define BULK_PERFT
uint64_t perft(board *brd, int depth) {
   arr_move256 move_list = new_arr_move256();
   if (depth == 1) {
      get_legal_moves(brd, &move_list);
      return move_list.size;
   }
   get_pseudolegal_moves(brd, &move_list);

   uint64_t moves = 0;
   for (uint32_t i = 0; i < move_list.size; i++) {
      move mov = arr_get_move256(&move_list, i);
      make_move(brd, mov);
      if (!prev_wasnt_legal(brd, !brd->to_play)) {
         moves += perft(brd, depth - 1);
      }
      undo_move(brd, mov);
   }

   return moves;
}

#define print_moves true
uint64_t perft_debug(FILE *f, board *brd, int depth) {
   arr_move256 move_list = new_arr_move256();
   if (depth == 0) {
      return 1;
   }

   if (depth == 1) {
      get_legal_moves(brd, &move_list);
      if (print_moves) {
         for (uint32_t i = 0; i < move_list.size; i++) {
            move mov = arr_get_move256(&move_list, i);
            uint64_t pcount = perft_debug(f, brd, 0);
            print_move_short(f, mov, pcount);
         }
      }
      return move_list.size;
   }
   get_pseudolegal_moves(brd, &move_list);

   // get_pseudolegal_moves(brd, &move_list);
   uint64_t moves = 0;
   for (uint32_t i = 0; i < move_list.size; i++) {
      move mov = arr_get_move256(&move_list, i);
      make_move(brd, mov);
      if (!prev_wasnt_legal(brd, !brd->to_play)) {
         uint64_t pcount = perft(brd, depth - 1);
         if (print_moves) {
            print_move_short(f, mov, pcount);
         }
         moves += pcount;
      }
      undo_move(brd, mov);
   }

   return moves;
}
