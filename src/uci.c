#include "board.h"
#include "debug_io.h"
#include "movegen.h"
#include "uci.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
static int search = 0;
static int is_ready = 1;

void uci(board *brd) {
   FILE *debugout = fopen("www.txt", "w");
   char buffer[0x1000] = "";
   while (1) {
      fgets(buffer, sizeof(buffer), stdin);
      if (strcmp(buffer, "") == 0) {
         continue;
      }
      // fprintf(stderr, "%s", buffer);
      fprintf(debugout, "fastchess: %s", buffer);

      int len = strlen(buffer);
      while (isspace(buffer[len - 1]) && len) {
         buffer[len - 1] = '\0';
      }

      if (strcmp(buffer, "uci") == 0) {
         printf("id name bengan bot\n");
         printf("id author Leonard Jarlskog\n");
         // printf("option \n");
         printf("uciok\n");
         fprintf(debugout, "engine: %s", "id name bengan bot\n");
         fprintf(debugout, "engine: %s", "id author Leonard Jarlskog\n");
         fprintf(debugout, "engine: %s", "uciok\n");
      }

      if (strcmp(buffer, "isready") == 0) {
         // while (is_ready == 0) {
         //    sleep(1);
         // }
         printf("readyok\n");
         fprintf(debugout, "engine: %s", "readyok\n");
      }

      if (strcmp(buffer, "exit") == 0) {
         return;
      }

      if (strcmp(buffer, "ucinewgame") == 0) {
         *brd = gen_start_board();
         // printf("readyok\n");
      }

      if (strcmp(buffer, "print_internal") == 0) {
         print_board(brd);
      }

      if (strncmp(buffer, "position", strlen("position\0")) == 0) {
         char *offset = buffer + strlen("position \0");
         fprintf(debugout, "%s:%d %s\n", __FILE__, __LINE__, offset);

         if (strncmp(offset, "startpos", strlen("startpos\0")) == 0) {
            *brd = gen_start_board();
            fprintf(debugout, "asked to make startpos (%s:%d)\n", __FILE__,
                    __LINE__);

         } else if (strncmp(offset, "fen", strlen("fen\0")) == 0) {
            fprintf(debugout, "asked to interpret fen\n");
            offset += strlen("fen \0");
            from_fen(offset, brd);
            fprint_board(debugout, brd);
            fprintf(debugout, "white castle_right: %d\n",
                    arr_get_castle_right4096(&brd->white_castle_history,
                                             brd->ply_count));
            fprintf(debugout, "black castle_right: %d\n",
                    arr_get_castle_right4096(&brd->black_castle_history,
                                             brd->ply_count));
         }

         // fprintf(stderr, "%s\n", buffer);
         // printf("readyok\n");
         // fprintf(debugout, "engine: %s", "readyok\n");
      }

      if (strncmp(buffer, "perft", strlen("perft\0")) == 0) {
         // *brd = gen_start_board();
         fprintf(debugout, "asked to perform perft\n");
         board test = gen_start_board();
         fprintf(debugout, "board and start board bitboards are equal? %b\n",
                 board_cmp_bb(brd, &test));
         fprintf(debugout, "board and start board histories are equal? %b\n",
                 board_cmp_hist(brd, &test));

         char *offset = buffer + strlen("perft \0");
         int depth = atoi(offset);
         fprintf(debugout, "going to perft on depth %d\n", depth);

         // fclose(debugout);
         uint64_t count = perft_debug(debugout, brd, depth);
         printf("%lu\n", count);
         fprintf(debugout, "perft resulted in %lu nodes\n", count);
      }

      if (strncmp(buffer, "go", strlen("go\0")) == 0) {
         exit(0);
      }

      if (strncmp(buffer, "quit", strlen("quit\0")) == 0) {
         exit(EXIT_SUCCESS);
      }

      fflush(debugout);
      fflush(stdout);
      memset(buffer, 0, sizeof(buffer));
   }
   fclose(debugout);
   // fclose(debugout);
}

void *start_uci(void *argp) {
   uci((board *)argp);
   return 0;
}
