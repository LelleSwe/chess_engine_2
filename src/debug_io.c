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
void print_board(board *_board) {
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

void print_move(move *_move) {
   char piece;

   switch (_move->piece) {
   case 0:
      piece = 'p';
      break;
   case 1:
      piece = 'b';
      break;
   case 2:
      piece = 'r';
      break;
   case 3:
      piece = 'q';
      break;
   case 4:
      piece = 'k';
      break;
   case 5:
      piece = 'n';
      break;
   default:
      piece = '.';
      break;
   }

   if (!_move->to_play && piece != '.') {
      piece -= 32;
   }

   char from_str[65];
   repeat(from_str, 64, '.');
   from_str[__builtin_clzl(_move->from)] = piece;

   char to_str[65];
   repeat(to_str, 64, '.');
   to_str[__builtin_clzl(_move->to)] = piece;

   printf("Moving from:");
   print_board_str(from_str);

   printf("Moving to:");
   print_board_str(to_str);
}
