#include "lib/mainLib.hpp"
#include "lib/shell.hpp"

unsigned int CURRENT_LINE = 0;
unsigned int CURRENT_COL = 0;
unsigned int CURRENT_COLOR = WHITE_TXT;

extern "C" void kernel_main() {
    kernel_clear_screen();

    arikkoShell();

    while(1) { __asm__ volatile("hlt"); };
}