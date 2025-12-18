#include "board.h"
#include "debug_io.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
static int search = 0;
static int is_ready = 1;

void uci(board *_board) {
   char buffer[0x100] = "";
   while (1) {
      fgets(buffer, sizeof(buffer), stdin);
      if (strcmp(buffer, "") == 0) {
         continue;
      }

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
         while (is_ready == 0) {
            sleep(1);
         }
         printf("readyok\n");
      }

      if (strcmp(buffer, "exit") == 0) {
         return;
      }

      if (strcmp(buffer, "ucinewgame") == 0) {
         memcpy(_board, &default_start_board, sizeof(board));
         printf("readyok\n");
      }

      if (strcmp(buffer, "print_internal") == 0) {
         print_board(_board);
      }

      if (strncmp(buffer, "position", strlen("position")) == 0) {
         from_long_algebraic(buffer + strlen("position \0"), _board);
      }

      fflush(stdout);
      memset(buffer, 0, sizeof(buffer));
   }
}

void *start_uci(void *argp) {
   uci((board *)argp);
   return 0;
}
