#include "board.h"
#include <stdio.h>

void print_board_str(char *str) {
   for (int i = 0; i < 64; i++) {
      if ((i % 8) == 0) {
         printf("\n%d  ", 8 - (i + 1) / 8);
      }

      char what = str[i];
      printf("%c", what);
   }
   puts("\n\n   ABCDEFGH\n");
}

/// Doesn't work. Idk why.
void print_board(const board *_board) {
   char out[65];
   out[64] = '\0';

   for (int row = 7; row >= 0; row--) {
      for (int col = 0; col < 8; col++) {
         int mask_offset = 8 * row + col;
         int assign_offset = 8 * (7 - row) + col;
         if (_board->wpa_bb & (bitboa)1 << mask_offset) {
            out[assign_offset] = 'P';
            // printf("P, %d", i);
         } else if (_board->wbi_bb & (bitboa)1 << mask_offset) {
            out[assign_offset] = 'B';
            // printf("B");
         } else if (_board->wro_bb & (bitboa)1 << mask_offset) {
            out[assign_offset] = 'R';
            // printf("R");
         } else if (_board->wqu_bb & (bitboa)1 << mask_offset) {
            out[assign_offset] = 'Q';
            // printf("Q");
         } else if (_board->wki_bb & (bitboa)1 << mask_offset) {
            out[assign_offset] = 'K';
            // printf("K");
         } else if (_board->wkn_bb & (bitboa)1 << mask_offset) {
            out[assign_offset] = 'N';
            // printf("N");
         } else if (_board->bpa_bb & (bitboa)1 << mask_offset) {
            out[assign_offset] = 'p';
         } else if (_board->bbi_bb & (bitboa)1 << mask_offset) {
            out[assign_offset] = 'b';
         } else if (_board->bro_bb & (bitboa)1 << mask_offset) {
            out[assign_offset] = 'r';
         } else if (_board->bqu_bb & (bitboa)1 << mask_offset) {
            out[assign_offset] = 'q';
         } else if (_board->bki_bb & (bitboa)1 << mask_offset) {
            out[assign_offset] = 'k';
         } else if (_board->bkn_bb & (bitboa)1 << mask_offset) {
            out[assign_offset] = 'n';
         } else {
            out[assign_offset] = '.';
            // printf(".");
         }
      }
   }

   // printf("out: %s\n", out);
   // printf("sizeof(bitboa): %lu", sizeof(bitboa));
   /*printf("wpa %lX\n", board->wpa_bb);
   printf("wbi %lX\n", board->wbi_bb);
   printf("wro %lX\n", board->wro_bb);
   printf("wqu %lX\n", board->wqu_bb);
   printf("wki %lX\n", board->wki_bb);
   printf("wkn %lX\n", board->wkn_bb);
   printf("bpa %lX\n", board->bpa_bb);
   printf("bbi %lX\n", board->bbi_bb);
   printf("bro %lX\n", board->bro_bb);
   printf("bqu %lX\n", board->bqu_bb);
   printf("bki %lX\n", board->bki_bb);
   printf("bkn %lX\n", board->bkn_bb);*/

   // meh();
   print_board_str(out);
}

void repeat(char *str, int len, char to) {
   for (int i = 0; i < len; i++) {
      str[i] = to;
   }
}

void print_move(const move *_move) {
   char pc;

   switch (_move->pc) {
   case PAWN:
      pc = 'p';
      break;
   case BISHOP:
      pc = 'b';
      break;
   case ROOK:
      pc = 'r';
      break;
   case QUEEN:
      pc = 'q';
      break;
   case KING:
      pc = 'k';
      break;
   case KNIGHT:
      pc = 'n';
      break;
   default:
      pc = '.';
      break;
   }

   if (!_move->to_play && pc != '.') {
      pc -= 32;
   }

   char from_str[65];
   repeat(from_str, 64, '.');
   char to_str[65];
   repeat(to_str, 64, '.');

   for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
         int mask_offset = 8 * row + col;
         int assign_offset = 8 * (7 - row) + col;
         if ((1UL << (mask_offset)) & _move->from)
            from_str[assign_offset] = pc;
         if ((1UL << (mask_offset)) & _move->to)
            to_str[assign_offset] = pc;
      }
   }

   printf("Moving from:");
   print_board_str(from_str);

   printf("Moving to:");
   print_board_str(to_str);
}

void print_board_internal(const board *brd) {
   printf("wpa_bb: %lu \nwbi_bb: %lu \nwro_bb: %lu \nwqu_bb: %lu \nwki_bb: %lu "
          "\nwkn_bb: %lu \nwcb_bb: %lu \nbpa_bb: %lu \nbbi_bb: %lu \nbro_bb: "
          "%lu \nbqu_bb: %lu \nbki_bb: %lu "
          "\nbkn_bb: %lu \nbcb_bb: %lu \nwhite_castle: %d \nblack_castle: %d "
          "\nenp: %lu\n\n",
          brd->wpa_bb, brd->wbi_bb, brd->wro_bb, brd->wqu_bb, brd->wki_bb,
          brd->wkn_bb, brd->wcb_bb, brd->bpa_bb, brd->bbi_bb, brd->bro_bb,
          brd->bqu_bb, brd->bki_bb, brd->bkn_bb, brd->bcb_bb, brd->white_castle,
          brd->black_castle, brd->enp);
}
void print_mov_internal(const move *mov) {
   printf("from: %lu \nto: %lu \npc: %d \ncapture: %d \npromotion: %d "
          "\ncastle: %d \nto_play: %d \nen_passant: %lu "
          "\nprev_en_passant: %lu\n\n",
          mov->from, mov->to, mov->pc, mov->capture, mov->promotion,
          mov->castle, mov->to_play, mov->en_passant, mov->prev_en_passant);
}
