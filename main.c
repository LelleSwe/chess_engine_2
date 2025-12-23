#include "src/board.h"
#include "src/debug_io.h"
#include "src/uci.h"
#include <pthread.h>

int main() {
   // Generate starting board
   board brd = gen_start_board();
   vec_move moves = new_vec_move(1000);
   pthargs arg = {&brd, &moves};

   // Start UCI thread
   pthread_t uci_id = 69;
   pthread_create(&uci_id, NULL, start_uci, &arg);
   pthread_join(uci_id, NULL);

   // print_board(&board);
   return 0;
}
