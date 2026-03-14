#ifndef INPUTLIB_HPP
#define INPUTLIB_HPP

#include "cursorLib.hpp"

extern unsigned int CURRENT_LINE;
extern unsigned int CURRENT_COL;
extern unsigned int CURRENT_COLOR;

inline void k_printf_at(const char* message, unsigned int line, unsigned int col, unsigned int color) {
    char* vidmem = (char*)0xb8000;
    unsigned int i = (line * 80 + col) * 2;

    while (*message != 0) {
        if (i >= 4000) break;

        if (*message == '\n') {
            line++;
            CURRENT_LINE = line;
            col = 0;
            i = (line * 80 + col) * 2;
        } else {
            vidmem[i++] = *message;
            vidmem[i++] = (unsigned char)(color);
            col++;
        }
        message++;
    }

    CURRENT_COL = col;
    unsigned int final_x = (i / 2) % 80;
    unsigned int final_y = (i / 2) / 80;
    update_cursor((int)(final_x), (int)(final_y));
}


inline unsigned char k_getch() {
    unsigned char scancode;
    unsigned char status;

    while (1) {
        asm volatile ("inb $0x64, %0" : "=a"(status));

        if (status & 0x01) {
            asm volatile ("inb $0x60, %0" : "=a"(scancode));

            if (!(scancode & 0x80)) {
                unsigned char ascii = (unsigned char)(scancode_ascii[scancode]);
                if (ascii) return ascii;
            }
        }
    }
}


inline void k_gets(char* buffer, unsigned int max_size) {
    unsigned int i = 0;
    unsigned short pos = get_cursor_position();
    unsigned int line = pos / 80;
    unsigned int current_col = pos % 80;

    while (i < max_size - 1) {
        unsigned char c = k_getch();

        if (c == '\n' || c == '\r') {
            buffer[i] = '\0';
            CURRENT_LINE = line + 1;
            CURRENT_COL = 0;
            update_cursor(0, (int)(line + 1));
            break;
        } 

        else if (c == '\b') {
            if (i > 0) {
                i--;
                current_col--;
                k_printf_at(" ", line, current_col, CURRENT_COLOR);
                update_cursor((int)(current_col), (int)(line));
            }
        } 

        else {
            buffer[i] = (char)(c);
            char temp[2] = { (char)(c), '\0' };
            
            k_printf_at(temp, line, current_col, CURRENT_COLOR);
            
            i++;
            current_col++;
            update_cursor((int)(current_col), (int)(line));
        }
    }
    CURRENT_LINE = line;
    CURRENT_COL = current_col;
}

#endif