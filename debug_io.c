#include "board.h"
#include <stdio.h>

void meh() {
    printf("w");
}

/// Doesn't work. Idk why.
void print_board(board* board) {
    char out[65] = "";
    out[64] = '\0';

    for (int i = 0; i < 64; i++) {
        if (board->wpa_bb & (1 << i)) {
            out[63-i] = 'P';
            //printf("P");
        }
        else if (board->wbi_bb & (1 << i)) {
            out[63-i] = 'B';
            //printf("B");
        }
        else if (board->wro_bb & (1 << i)) {
            out[63-i] = 'R';
            //printf("R");
        }
        else if (board->wqu_bb & (1 << i)) {
            out[63-i] = 'Q';
            //printf("Q");
        }
        else if (board->wki_bb & (1 << i)) {
            out[63-i] = 'K';
            //printf("K");
        }
        else if (board->wkn_bb & (1 << i)) {
            out[63-i] = 'N';
            //printf("N");
        }
        else if (board->bpa_bb & (1 << i)) {
            out[63-i] = 'P';
        }
        else if (board->bbi_bb & (1 << i)) {
            out[63-i] = 'B';
        }
        else if (board->bro_bb & (1 << i)) {
            out[63-i] = 'R';
        }
        else if (board->bqu_bb & (1 << i)) {
            out[63-i] = 'Q';
        }
        else if (board->bki_bb & (1 << i)) {
            out[63-i] = 'K';
        }
        else if (board->bkn_bb & (1 << i)) {
            out[63-i] = 'N';
        }
        else {
            out[63-i] = '.';
            //printf(".");
        }
    }

    //printf("out: %s\n", out);
    //printf("sizeof(bitboa): %lu", sizeof(bitboa));
    printf("wpa %llX\n", board->wpa_bb);
    printf("wbi %llX\n", board->wbi_bb);
    printf("wro %llX\n", board->wro_bb);
    printf("wqu %llX\n", board->wqu_bb);
    printf("wki %llX\n", board->wki_bb);
    printf("wkn %llX\n", board->wkn_bb);
    printf("bpa %llX\n", board->bpa_bb);
    printf("bbi %llX\n", board->bbi_bb);
    printf("bro %llX\n", board->bro_bb);
    printf("bqu %llX\n", board->bqu_bb);
    printf("bki %llX\n", board->bki_bb);
    printf("bkn %llX\n", board->bkn_bb);

    //meh();
    for (int i = 0; i < 64; i++) {
        if ((i % 8) == 0) {
            printf("\n%d ", (i+1)/8);
        }

        char what = out[i];
        printf("%c", what);
    }
}