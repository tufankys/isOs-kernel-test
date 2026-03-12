#ifndef MAINLIB_H
#define MAINLIB_H

#define BLACK_TXT 0x00
#define BLUE_TXT 0x01
#define GREEN_TXT 0x02
#define CYAN_TXT 0x03
#define RED_TXT 0x04
#define PURPLE_TXT 0x05
#define BROWN_TXT 0x06
#define WHITE_TXT 0x07
#define GRAY_TXT 0x08
#define LIGHTBLUE_TXT 0x09

unsigned int CURRENT_LINE = 0;
unsigned int CURRENT_COL = 0;

#include "inputLib.h"

int strcmp(char *s1, char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

#endif
