#ifndef CURSORLIB_H
#define CURSORLIB_H

//mouse olan cursor değil yanıp sönen şey bu

static inline unsigned char inb(unsigned short port) {
    unsigned char data;
    asm volatile ("inb %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

static inline void outb(unsigned short port, unsigned char data) {
    asm volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}


unsigned short get_cursor_position() {
    unsigned short pos = 0;
    asm volatile("outb %0, %1" : : "a"((unsigned char)0x0F), "Nd"(0x3D4));
    pos |= (unsigned char)inb(0x3D5);

    asm volatile("outb %0, %1" : : "a"((unsigned char)0x0E), "Nd"(0x3D4));
    pos |= ((unsigned short)inb(0x3D5) << 8);
    
    return pos;
}

void update_cursor(int x, int y) {
    unsigned short pos = y * 80 + x;

    asm volatile("outb %0, %1" : : "a"((unsigned char)0x0F), "Nd"(0x3D4));
    asm volatile("outb %0, %1" : : "a"((unsigned char)(pos & 0xFF)), "Nd"(0x3D5));

    asm volatile("outb %0, %1" : : "a"((unsigned char)0x0E), "Nd"(0x3D4));
    asm volatile("outb %0, %1" : : "a"((unsigned char)((pos >> 8) & 0xFF)), "Nd"(0x3D5));
}

#endif