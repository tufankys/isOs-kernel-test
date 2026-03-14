#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "mainLib.hpp"

extern unsigned int CURRENT_LINE;
extern unsigned int CURRENT_COL;
extern unsigned int CURRENT_COLOR;

typedef void (*IslevPointer)(char* words[MAX_WORDS]);

struct tableStruct {
    const char* key;
    IslevPointer function;
};

//////////////////////////////////////////////

void ping(char* words[MAX_WORDS]) {
    k_printf_at("\nPong!\n", CURRENT_LINE, CURRENT_COL, CURRENT_COLOR);
}

void clear(char* words[MAX_WORDS]) {
    kernel_clear_screen();
}

void color(char* words[MAX_WORDS]) {
    char wordCount[12];
    toString(getWordsLength(words), wordCount);

    CURRENT_COLOR = hexStringToDecimal(words[1]);
    k_printf_at("\nSet the color to: ", CURRENT_LINE, CURRENT_COL, CURRENT_COLOR);
    k_printf_at(words[1], CURRENT_LINE, CURRENT_COL, CURRENT_COLOR);
    k_printf_at("\n", CURRENT_LINE, CURRENT_COL, CURRENT_COLOR);
}

void echo(char* words[MAX_WORDS]) {
    int count = getWordsLength(words);
    char temp[12];

    k_printf_at("\n", CURRENT_LINE, CURRENT_COL, CURRENT_COLOR);
    for (int i = 0; i < count; i++) {
        if (i == 0) continue;
        k_printf_at(words[i], CURRENT_LINE, CURRENT_COL, CURRENT_COLOR);
        k_printf_at(" ", CURRENT_LINE, CURRENT_COL, CURRENT_COLOR);
    }
    k_printf_at("\n", CURRENT_LINE, CURRENT_COL, CURRENT_COLOR);
}

//////////////////////////////////////////////

tableStruct commandTable[] = {
    {"ping", ping},
    {"clear", clear},
    {"color", color},
    {"echo", echo}
};

//////////////////////////////////////////////

void searchCommand(char* searching, char* words[MAX_WORDS]) {
    int tabloBoyutu = sizeof(commandTable) / sizeof(commandTable[0]);

    for (int i = 0; i < tabloBoyutu; i++) {
        if (strcmp(searching, commandTable[i].key) == 0) {
            commandTable[i].function(words);
            return;
        }
    }
    
    k_printf_at("\nCommand not found: ", CURRENT_LINE, CURRENT_COL, CURRENT_COLOR);
    k_printf_at(words[0], CURRENT_LINE, CURRENT_COL, CURRENT_COLOR);
    k_printf_at("\n", CURRENT_LINE, CURRENT_COL, CURRENT_COLOR);
}

#endif