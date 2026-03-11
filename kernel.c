#include "library.h"
unsigned int CURRENT_LINE = 0;

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

unsigned int k_printf(char *message, unsigned int line, unsigned int color) {
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;

	i=(line*80*2);

	while(*message!=0) {
		if(*message=='\n') {
			line++;
			i=(line*80*2);
			*message++;
		} else {
			vidmem[i]=*message;
			*message++;
			i++;
			vidmem[i]=color;
			i++;
		};
	};

	CURRENT_LINE = CURRENT_LINE + 1;
	return(1);
}


unsigned char k_getch() {
    unsigned char scancode;
    unsigned char ascii = 0;

    while(1) {
        asm volatile (
            "inb $0x60, %0"
            : "=a"(scancode)
        );

        if(scancode & 0x80) { continue; }

        if(scancode < 128) {
            ascii = scancode_ascii[scancode];
            if(ascii) { return ascii; }
        }
    }
}

void k_main()  {
	k_clear_screen();
	k_printf("Hello, World!", CURRENT_LINE, WHITE_TXT);
	
	//delete beneath this comment if you dont want getch
	unsigned char pressed;
	pressed = k_getch();
	char translated[2] = { pressed, 0 };
    k_printf(translated, CURRENT_LINE, WHITE_TXT);
	//
};