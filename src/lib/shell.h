#ifndef SHELL_H
#define SHELL_H

#include "mainLib.h"

extern unsigned int CURRENT_LINE;
extern unsigned int CURRENT_COL;

void arikkoShell() {
    while (1) {
        k_printf_at("arikko@kernel/> ", CURRENT_LINE, CURRENT_COL, WHITE_TXT);
        char input[64];
	    k_gets(input, 64);

        char *words[MAX_WORDS];
        int count = 0;
        char countString[12];

        split_and_store(input, words, &count);
        toString(count, countString);
        
        if (count > 0) {
            if (strcmp(words[0], "ping") == 0) {
                k_printf_at("Pong!", CURRENT_LINE, CURRENT_COL, WHITE_TXT);
                k_printf_at("\n", CURRENT_LINE, CURRENT_COL, WHITE_TXT);
            } else if (strcmp(words[0], "hello") == 0) {
                k_printf_at("World!", CURRENT_LINE, CURRENT_COL, WHITE_TXT);
                k_printf_at("\n", CURRENT_LINE, CURRENT_COL, WHITE_TXT);
            } else {
                k_printf_at("Command not found: ", CURRENT_LINE, CURRENT_COL, WHITE_TXT);
                k_printf_at(words[0], CURRENT_LINE, CURRENT_COL, WHITE_TXT);
                k_printf_at("\n", CURRENT_LINE, CURRENT_COL, WHITE_TXT);
            }    
        } 

    }
    
}

#endif