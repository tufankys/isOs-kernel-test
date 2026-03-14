#include "lib/mainLib.hpp"
#include "lib/shell.hpp"

unsigned int CURRENT_LINE = 0;
unsigned int CURRENT_COL = 0;

void kernel_clear_screen() {
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	while(i < (80*25*2)) {
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};

	update_cursor(CURRENT_COL, CURRENT_LINE);
};

extern "C" void kernel_main() {
    kernel_clear_screen();

    arikkoShell();

    while(1) { __asm__ volatile("hlt"); };
}