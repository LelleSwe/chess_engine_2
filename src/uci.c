#include "debug_io.h"
#include "movegen.h"
#include "search.h"
#include "uci.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void uci(board *brd) {
   // FILE *debugout = fopen("www.txt", "w");
   char buffer[0x1000] = "";
   search_res result;
   while (1) {
      fgets(buffer, sizeof(buffer), stdin);
      if (strcmp(buffer, "") == 0) {
         continue;
      }
      // fprintf(debugout, "fastchess: %s", buffer);

      int len = strlen(buffer);
      while (isspace(buffer[len - 1]) && len) {
         buffer[len - 1] = '\0';
      }

      if (strcmp(buffer, "uci") == 0) {
         printf("id name bengan bot\n");
         printf("id author Leonard Jarlskog\n");
         printf("uciok\n");
      }

      if (strcmp(buffer, "isready") == 0) {
         printf("readyok\n");
      }

      if (strcmp(buffer, "exit") == 0) {
         return;
      }

      if (strcmp(buffer, "ucinewgame") == 0) {
         *brd = gen_start_board();
      }

      if (strcmp(buffer, "print_internal") == 0) {
         print_board(brd);
         print_board_internal(brd);
      }

      if (strncmp(buffer, "position", strlen("position\0")) == 0) {
         char *offset = buffer + strlen("position \0");

         if (strncmp(offset, "startpos", strlen("startpos\0")) == 0) {
            *brd = gen_start_board();

         } else if (strncmp(offset, "fen", strlen("fen\0")) == 0) {
            offset += strlen("fen \0");
            from_fen(offset, brd);
         }
      }

      if (strncmp(buffer, "moves", strlen("moves\0")) == 0) {
         char *offset = buffer + strlen("moves \0");
         from_long_algebraic(offset, brd);
      }

      if (strncmp(buffer, "perft", strlen("perft\0")) == 0) {
         char *offset = buffer + strlen("perft \0");
         int depth = atoi(offset);

         uint64_t count = perft(brd, depth);
         printf("%lu\n", count);
      }

      if (strncmp(buffer, "go", strlen("go\0")) == 0) {
         result = search(brd);
         char out[6] = "";
         format_move(out, result.mov);
         printf("bestmove %s\n", out);
      }

      if (strncmp(buffer, "quit", strlen("quit\0")) == 0) {
         exit(EXIT_SUCCESS);
      }

      if (strncmp(buffer, "info", strlen("info\0")) == 0) {
         printf("info score %d depth %d", result.score, result.depth);
      }

      // fflush(debugout);
      fflush(stdout);
      memset(buffer, 0, sizeof(buffer));
   }
   // fclose(debugout);
   // fclose(debugout);
}

void *start_uci(void *argp) {
   uci((board *)argp);
   return 0;
}
