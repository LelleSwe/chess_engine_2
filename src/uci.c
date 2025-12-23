#include "board.h"
#include "debug_io.h"
#include "uci.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
static int search = 0;
static int is_ready = 1;

void uci(board *brd, vec_move *moves) {
   char buffer[0x1000] = "";
   while (1) {
      fgets(buffer, sizeof(buffer), stdin);
      if (strcmp(buffer, "") == 0) {
         continue;
      }
      // fprintf(stderr, "%s", buffer);

      int len = strlen(buffer);
      while (isspace(buffer[len - 1]) && len) {
         buffer[len - 1] = '\0';
      }

      if (strcmp(buffer, "uci") == 0) {
         printf("id name bengan bot\n");
         printf("id author Leonard Jarlskog\n");
         // printf("option \n");
         printf("uciok\n");
      }

      if (strcmp(buffer, "isready") == 0) {
         // while (is_ready == 0) {
         //    sleep(1);
         // }
         printf("readyok\n");
      }

      if (strcmp(buffer, "exit") == 0) {
         return;
      }

      if (strcmp(buffer, "ucinewgame") == 0) {
         memcpy(brd, &default_start_board, sizeof(board));
         printf("readyok\n");
      }

      if (strcmp(buffer, "print_internal") == 0) {
         print_board(brd);
      }

      if (strncmp(buffer, "position", strlen("position")) == 0) {
         from_long_algebraic(buffer + strlen("position \0"), brd, moves);
         fprintf(stderr, "%s\n", buffer);
         printf("readyok\n");
      }

      fflush(stdout);
      memset(buffer, 0, sizeof(buffer));
   }
}

void *start_uci(void *argp) {
   pthargs *args = (pthargs *)argp;
   uci(args->brd, args->moves);
   return 0;
}
