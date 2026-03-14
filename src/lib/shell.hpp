#ifndef SHELL_HPP
#define SHELL_HPP

#include "commands.hpp"


extern unsigned int CURRENT_LINE;
extern unsigned int CURRENT_COL;
extern unsigned int CURRENT_COLOR;

inline void arikkoShell() {
    while (true) {
        k_printf_at("arikko@kernel/> ", CURRENT_LINE, CURRENT_COL, CURRENT_COLOR);
        
        char input[64];
        k_gets(input, 64);

        char* words[MAX_WORDS];
        int count = 0;
        //char countString[12]; 

        split_and_store(input, words, &count);
        //toString(count, countString);
        
        if (count > 0) { 
            searchCommand(words[0], words); 
        } else {
            k_printf_at("\n", CURRENT_LINE, CURRENT_COL, CURRENT_COLOR);
        }
        
    }
}

#endif