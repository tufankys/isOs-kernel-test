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

#define MAX_WORDS 16
#define KEYBOARD_BUFFER_SIZE 256

#include "inputLib.h"

static int strcmp(char *s1, char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

static void split_and_store(char *str, char **word_list, int *word_count) {
    if (str == 0 || word_list == 0) return;

    char *ptr = str;
    int count = 0;
    int in_word = 0;

    while (*ptr != '\0' && count < MAX_WORDS) {
        if (*ptr == ' ') {
            *ptr = '\0';
            in_word = 0;
        } else {
            if (in_word == 0) {
                word_list[count] = ptr;
                count++;
                in_word = 1;
            }
        }
        ptr++;
    }
    *word_count = count;
}

static void toString(int num, char *str) {
    int i = 0;
    int is_negative = 0;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    while (num != 0) {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num / 10;
    }

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

#endif
