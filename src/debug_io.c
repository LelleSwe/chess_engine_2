#include "board.h"
#include <stdio.h>

void meh() { printf("w"); }

void print_board_str(char *str) {
   for (int i = 0; i < 64; i++) {
      if ((i % 8) == 0) {
         printf("\n%d ", (i + 1) / 8);
      }

      char what = str[i];
      printf("%c", what);
   }
   puts("\n");
}

/// Doesn't work. Idk why.
void print_board(const board *_board) {
   char out[65];
   out[64] = '\0';

   for (int i = 0; i < 64; i++) {
      if (_board->wpa_bb & (bitboa)1 << i) {
         out[63 - i] = 'P';
         // printf("P, %d", i);
      } else if (_board->wbi_bb & (bitboa)1 << i) {
         out[63 - i] = 'B';
         // printf("B");
      } else if (_board->wro_bb & (bitboa)1 << i) {
         out[63 - i] = 'R';
         // printf("R");
      } else if (_board->wqu_bb & (bitboa)1 << i) {
         out[63 - i] = 'Q';
         // printf("Q");
      } else if (_board->wki_bb & (bitboa)1 << i) {
         out[63 - i] = 'K';
         // printf("K");
      } else if (_board->wkn_bb & (bitboa)1 << i) {
         out[63 - i] = 'N';
         // printf("N");
      } else if (_board->bpa_bb & (bitboa)1 << i) {
         out[63 - i] = 'p';
      } else if (_board->bbi_bb & (bitboa)1 << i) {
         out[63 - i] = 'b';
      } else if (_board->bro_bb & (bitboa)1 << i) {
         out[63 - i] = 'r';
      } else if (_board->bqu_bb & (bitboa)1 << i) {
         out[63 - i] = 'q';
      } else if (_board->bki_bb & (bitboa)1 << i) {
         out[63 - i] = 'k';
      } else if (_board->bkn_bb & (bitboa)1 << i) {
         out[63 - i] = 'n';
      } else {
         out[63 - i] = '.';
         // printf(".");
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
   from_str[__builtin_clzl(_move->from)] = pc;

   char to_str[65];
   repeat(to_str, 64, '.');
   to_str[__builtin_clzl(_move->to)] = pc;

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
