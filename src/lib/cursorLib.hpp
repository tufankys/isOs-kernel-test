#ifndef CURSORLIB_HPP
#define CURSORLIB_HPP

extern unsigned int CURRENT_LINE;
extern unsigned int CURRENT_COL;

static const char scancode_ascii[128] = { 0, 27, '1','2','3','4','5','6','7','8','9','0','-','=','\b', '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0, 'a','s','d','f','g','h','j','k','l',';','\'',' ', 0,'\\','z','x', 'c','v','b','n','m',',','.','/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static inline void outb(unsigned short port, unsigned char data) {
    __asm__ volatile ("outb %b0, %w1" : : "a"(data), "Nd"(port));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char data;
    __asm__ volatile ("inb %w1, %b0" : "=a"(data) : "Nd"(port));
    return data;
}


static unsigned short get_cursor_position(void) {
    unsigned short pos = 0;

    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);

    outb(0x3D4, 0x0E);
    pos |= ((unsigned short)inb(0x3D5) << 8);

    return pos;
}

static void update_cursor(int x, int y) {
    unsigned short pos = (unsigned short)(y * 80 + x);

    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

#endif