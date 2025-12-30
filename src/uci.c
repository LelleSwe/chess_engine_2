#include "debug_io.h"
#include "movegen.h"
#include "search.h"
#include "uci.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void uci(board *brd) {
   setvbuf(stdin, nullptr, _IONBF, 0);
   setvbuf(stdout, nullptr, _IONBF, 0);
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
            offset += strlen("startpos \0");
            *brd = gen_start_board();
         }
         if (strncmp(offset, "fen", strlen("fen\0")) == 0) {
            offset += strlen("fen \0");
            offset += from_fen(offset, brd);
            offset++;
         }
         if (strncmp(offset, "moves", strlen("moves\0")) == 0) {
            offset += strlen("moves \0");
            from_long_algebraic(offset, brd);
         }
      }

      if (strncmp(buffer, "moves", strlen("moves\0")) == 0) {
         char *offset = buffer + strlen("moves \0");
         from_long_algebraic(offset, brd);
      }

      if (strncmp(buffer, "makebest", strlen("makebest\0")) == 0) {
         make_move(brd, result.mov);
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
         // print_res(&result);
         printf("bestmove %s\n", out);
      }

      if (strncmp(buffer, "quit", strlen("quit\0")) == 0) {
         exit(EXIT_SUCCESS);
      }

      if (strncmp(buffer, "info", strlen("info\0")) == 0) {
         printf("info score %d depth %d\n", result.score, result.depth);
      }

      if (strncmp(buffer, "playself", strlen("playself\n")) == 0) {
         result.mov = 1;
         char out[6] = "";
         while (result.mov != 0) {
            result = search(brd);
            format_move(out, result.mov);

            printf("%s ", out);
            fflush(stdout);
            make_move(brd, result.mov);
         };
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
