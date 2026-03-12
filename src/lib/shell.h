#ifndef SHELL_H
#define SHELL_H

#include "mainLib.h"

void arikkoShell() {
    while (1) {
        k_printf_at("arikko@kernel/> ", CURRENT_LINE, CURRENT_COL, WHITE_TXT);
        char input[64];
	    k_gets(input, 64);

        if (strcmp(input, "ping") == 0) {
            k_printf_at("Pong!", CURRENT_LINE, CURRENT_COL, WHITE_TXT);
            k_printf_at("\n", CURRENT_LINE, CURRENT_COL, WHITE_TXT);
        } else if (strcmp(input, "hello") == 0) {
            k_printf_at("World!", CURRENT_LINE, CURRENT_COL, WHITE_TXT);
            k_printf_at("\n", CURRENT_LINE, CURRENT_COL, WHITE_TXT);
        }

    }
    
}

#endif