#ifndef INPUTLIB_H
#define INPUTLIB_H

#include "cursorLib.h"

static const char scancode_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\','z','x',
    'c','v','b','n','m',',','.','/', 0,  '*', 0, ' ', 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, '+', 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


void k_printf(char *message, unsigned int line, unsigned int color) {
    char *vidmem = (char *) 0xb8000;
    unsigned int i;

    i = (line * 80 * 2);

    while(*message != 0) {
        if (i >= 4000) break; 

        if(*message == '\n') {
            line++;
            i = (line * 80 * 2);
        } else {
            vidmem[i++] = *message;
            vidmem[i++] = (char)color;
        }
        message++;
    }

	unsigned int final_x = (i / 2) % 80;
	unsigned int final_y = (i / 2) / 80;
	update_cursor(final_x, final_y);
}									
									
void k_printf_at(char *message, unsigned int line, unsigned int col, unsigned int color) {
    char *vidmem = (char *) 0xb8000;
    unsigned int i;

    i = (line * 80 + col) * 2;

    while(*message != 0) {
        if (i >= 4000) break;

        if(*message == '\n') {
            line++;
            CURRENT_LINE = line;
            col = 0;
            i = (line * 80 + col) * 2;
        } else {
            vidmem[i++] = *message;
            vidmem[i++] = (unsigned char)color;
            col++;
        }
        message++;
    }

    CURRENT_COL = col;

    unsigned int final_x = (i / 2) % 80;
	unsigned int final_y = (i / 2) / 80;
	update_cursor(final_x, final_y);
}

unsigned char k_getch() {
    unsigned char scancode;
    unsigned char status;

    while(1) {
        asm volatile ("inb $0x64, %0" : "=a"(status));

        if (status & 0x01) {
            asm volatile ("inb $0x60, %0" : "=a"(scancode));

            if (!(scancode & 0x80)) {
                unsigned char ascii = scancode_ascii[scancode];
                if (ascii) return ascii;
            }
        }

    }
}

void k_gets(char *buffer, unsigned int max_size) {
    unsigned int i = 0;
    unsigned short pos = get_cursor_position();
    unsigned int start_col = pos % 80;
    unsigned int line = pos / 80;
    unsigned int current_col = start_col;

    while (i < max_size - 1) {
        unsigned char c = k_getch();

        if (c == '\n' || c == '\r') {
            buffer[i] = '\0';
            CURRENT_LINE = line + 1;
            CURRENT_COL = 0;
            update_cursor(0, line + 1); 
            break;
        } 
        else if (c == '\b') {
            if (i > 0) {
                i--;
                current_col--;
                CURRENT_LINE = line;
                CURRENT_COL = current_col;
                k_printf_at(" ", line, current_col, 0x07);
                CURRENT_LINE = line;
                CURRENT_COL = current_col;
                update_cursor(current_col, line);
            }
        } 
        else {
            buffer[i] = c;
            char temp[2] = {c, 0};
            
            k_printf_at(temp, line, current_col, 0x07);
            
            i++;
            current_col++;
            CURRENT_LINE = line;
            CURRENT_COL = current_col;
            update_cursor(current_col, line);
        }
    }
}

#endif