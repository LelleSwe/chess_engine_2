#include "board.h"
#include "debug_io.c"
#include <pthread.h>
#include "uci.h"

int main() {
    board board = gen_start_board();
    
    pthread_t uci_id = 1;
    pthread_create(&uci_id, NULL, start_uci, NULL);
    //pthread_join(uci_id, NULL);

    print_board(&board);
    return 0;
}