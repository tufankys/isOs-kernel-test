#ifndef MAINLIB_HPP
#define MAINLIB_HPP

#include "inputLib.hpp"

#define BLACK_TXT      0x00
#define BLUE_TXT       0x01
#define GREEN_TXT      0x02
#define CYAN_TXT       0x03
#define RED_TXT        0x04
#define PURPLE_TXT     0x05
#define BROWN_TXT      0x06
#define WHITE_TXT      0x07
#define GRAY_TXT       0x08
#define LIGHTBLUE_TXT  0x09

#define MAX_WORDS 16
#define KEYBOARD_BUFFER_SIZE 256

void kernel_clear_screen() {
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	while(i < (80*25*2)) {
		vidmem[i]=' ';
		i++;
		vidmem[i]=CURRENT_COLOR;
		i++;
	};

    CURRENT_COL = 0;
    CURRENT_LINE = 0;

	update_cursor(CURRENT_COL, CURRENT_LINE);
};

inline int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (int)(*(const unsigned char*)s1) - 
           (int)(*(const unsigned char*)s2);
}


inline void split_and_store(char* str, char** word_list, int* word_count) {
    if (str == 0 || word_list == 0) return;

    char* ptr = str;
    int count = 0;
    int in_word = 0;

    while (*ptr != '\0' && count < MAX_WORDS) {
        if (*ptr == ' ') {
            *ptr = '\0';
            in_word = 0;
        } else {
            if (!in_word) {
                word_list[count] = ptr;
                count++;
                in_word = 1;
            }
        }
        ptr++;
    }
    *word_count = count;
}


inline char* toString(int num, char* str) {
    int i = 0;
    bool is_negative = false;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0) {
        is_negative = true;
        num = -num; 
    }

    while (num != 0) {
        int rem = num % 10;
        str[i++] = (char)(rem + '0');
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

    return str;
}

inline int toNumber(const char* str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
        i++;
    }

    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    while (str[i] >= '0' && str[i] <= '9') {
        result = (result * 10) + (str[i] - '0');
        i++;
    }

    return result * sign;
}

inline int hexStringToDecimal(char* hexStr) {
    int result = 0;
    int i = 0;

    // Eğer string "0x" veya "0X" ile başlıyorsa ilk iki karakteri atla
    if (hexStr[0] == '0' && (hexStr[1] == 'x' || hexStr[1] == 'X')) {
        i = 2;
    }

    while (hexStr[i] != '\0') {
        int value = 0;
        char c = hexStr[i];

        if (c >= '0' && c <= '9') {
            value = c - '0';        // '0'-'9' arası rakamlar
        } 
        else if (c >= 'a' && c <= 'f') {
            value = c - 'a' + 10;   // 'a'-'f' arası (küçük harf)
        } 
        else if (c >= 'A' && c <= 'F') {
            value = c - 'A' + 10;   // 'A'-'F' arası (büyük harf)
        } 
        else {
            // Geçersiz bir karakter gelirse döngüyü kırabilir veya hata dönebilirsiniz
            break; 
        }

        result = (result * 16) + value;
        i++;
    }

    return result;
}

inline int getWordsLength(char *words[MAX_WORDS]) {
    int count = 0;
    while (words[count] != nullptr) { // C kullanıyorsan NULL yazabilirsin
        count++;
    }
    return count;
}

#endif