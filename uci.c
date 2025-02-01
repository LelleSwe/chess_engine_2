#include <string.h>
#include <stdio.h>
static int search = 0;

void uci() {
    char buffer[0x100] = "";
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        if (strcmp(buffer, "") == 0) {
            continue;
        }
        //printf("%s", buffer);

        if (strcmp(buffer, "uci\n") == 0) {
            printf("id bengan_bot\n");
        }
        if (strcmp(buffer, "exit\n") == 0) {
            return;
        }


        memset(buffer, 0, sizeof(buffer));
    }
}

void* start_uci(void* argp) {
    uci();
    return 0;
}