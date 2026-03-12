#include "lib/mainLib.h"
#include "lib/shell.h"

//yav amına koyiyim commentları normalde çok ikonik oldukları için silmek istemezdim
//ama gözümü çok alıyolar diye sildim ama yinede gerektiğinde yaz sen
//bu arada niye bilmiyorum birsürü bilinmeyen karakter diye bişey vardı kodun içinde

void k_clear_screen() {
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	while(i < (80*25*2)) {
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};
};

void k_wait(unsigned int wait_limit) {
	volatile unsigned int x, y;
	for (x = 0; x < wait_limit; x++) {
        for (y = 0; y < 10000; y++) { 
            asm volatile("nop"); 
        }
    }
};

void k_main() {
	k_clear_screen();
    arikkoShell();
};

