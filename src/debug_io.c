#include "board.h"
#include "movegen.h"
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

void fprint_board_str(FILE *f, char *str) {
   for (int i = 0; i < 64; i++) {
      if ((i % 8) == 0) {
         fprintf(f, "\n%d  ", 8 - (i + 1) / 8);
      }

      char what = str[i];
      fprintf(f, "%c", what);
   }
   fprintf(f, "\n\n   ABCDEFGH\n\n");
}

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

   print_board_str(out);
}

void fprint_board(FILE *f, const board *_board) {
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

   fprint_board_str(f, out);
}
void repeat(char *str, int len, char to) {
   for (int i = 0; i < len; i++) {
      str[i] = to;
   }
}

void print_bitboard(const bitboa brd) {
   char pc = 'P';
   char out[65] = "";
   repeat(out, 64, '.');

   for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
         int mask_offset = 8 * row + col;
         int assign_offset = 8 * (7 - row) + col;
         if ((1UL << (mask_offset)) & brd)
            out[assign_offset] = pc;
      }
   }

   printf("printing bitboard: \n");
   print_board_str(out);
}

void print_move_short(FILE *f, const move mov, const uint64_t perft_count) {
   square from = from_move(mov);
   square to = to_move(mov);
   piece prom = promotion_move(mov);

   int frank = from / 8;
   int ffile = from % 8;
   char froms[3] = "";
   froms[0] = 'a' + ffile;
   froms[1] = '1' + frank;

   int trank = to / 8;
   int tfile = to % 8;
   char tos[3] = "";
   tos[0] = 'a' + tfile;
   tos[1] = '1' + trank;

   char promc = prom == NO_PIECE ? '\0' : 'Q';
   fprintf(f, "%s%s", froms, tos);
   if (promc != '\0') {
      fprintf(f, "%c", promc);
   }
   fprintf(f, " - %zu\n", perft_count);
   // printf("%s%s%c\n", froms, tos, promc);
}

void print_move(const move mov) {
   char pc = 'P';
   bitboa from = from_move(mov);
   bitboa to = to_move(mov);
   piece promote = promotion_move(mov);

   char from_str[65];
   repeat(from_str, 64, '.');
   char to_str[65];
   repeat(to_str, 64, '.');

   for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
         int mask_offset = 8 * row + col;
         int assign_offset = 8 * (7 - row) + col;
         if ((1UL << (mask_offset)) & from)
            from_str[assign_offset] = pc;
         if ((1UL << (mask_offset)) & to)
            to_str[assign_offset] = pc;
      }
   }

   printf("Moving from:");
   print_board_str(from_str);

   printf("Moving to:");
   print_board_str(to_str);

   printf("Promotion? %d\n", promote);
}

void print_board_internal(const board *brd) {
   printf("wpa_bb: %lu \nwbi_bb: %lu \nwro_bb: %lu \nwqu_bb: %lu \nwki_bb: %lu "
          "\nwkn_bb: %lu \nwcb_bb: %lu \nbpa_bb: %lu \nbbi_bb: %lu \nbro_bb: "
          "%lu \nbqu_bb: %lu \nbki_bb: %lu "
          "\nbkn_bb: %lu \nbcb_bb: %lu \nwhite_castle: %zu \nblack_castle: %zu "
          "\nenp: %lu \nmoves: %lu \ncaptures: %lu \n\n",
          brd->wpa_bb, brd->wbi_bb, brd->wro_bb, brd->wqu_bb, brd->wki_bb,
          brd->wkn_bb, brd->wcb_bb, brd->bpa_bb, brd->bbi_bb, brd->bro_bb,
          brd->bqu_bb, brd->bki_bb, brd->bkn_bb, brd->bcb_bb,
          brd->white_castle_history.size, brd->black_castle_history.size,
          brd->enp_history.size, brd->move_history.size,
          brd->capture_history.size);
}

#include <string.h>
void print_move_history(const board *brd) {
   arr_move4096 move_list = new_arr_move4096();
   memcpy(&move_list, &brd->move_history, sizeof(arr_move4096));

   board new = gen_start_board();
   // new.to_play = !new.to_play;

   for (uint32_t i = 1; i < move_list.size; i++) {
      move mov = arr_get_move4096(&move_list, i);
      print_board(&new);
      printf("Attacking bitboard: \n");
      print_bitboard(get_attack_bb(&new));
      printf("White in check? %b\n", in_check(&new, false));
      printf("Black in check? %b\n", in_check(&new, true));
      // print_move(mov);
      make_move(&new, mov);
   }
   print_board(&new);
}
