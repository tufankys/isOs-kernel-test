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

void k_wait(unsigned int wait_limit) {	 // bu kod öküz gibi verimsiz bizimki gibi pc lerde belki 0.1 saniye sürerken eskilerde 1 saniye bile sürebilir o yüzzden şimdilik PIT ye erişimin olmadığı
	volatile unsigned int x, y;	 // varsayıyorum o yüzxden şimdilik kullan diye bunu koyuyorum  bunla kodu dizersen seni patlatırım
	for (x = 0; x < wait_limit; x++) {
        for (y = 0; y < 10000; y++) { 
            asm volatile("nop"); 
        }
    }
};

unsigned int k_printf(char *message, unsigned int line, unsigned int color) {
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;

	i=(line*80*2);

	while(*message!=0) {
		if(*message=='\n') {
			line++;
			i=(line*80*2);
			message++; // Burayı düzelttim yoksa sonsuz döngüye girerdi kral
		} else {
			vidmem[i]=*message;
			message++;
			i++;
			vidmem[i]=color;
			i++;
		};
	};

	CURRENT_LINE = line + 1;  //haberin olsun diye diyorum ben şimdi buı kodda 5 tane \n kullanırsam bu sayaç yine 1 artıcak ona göre fixlersin burayı ben eğlenceli kısımlara
	return(1);						  //ekleme yapmak için yapıyorum bu commiti sonra yok sıkıntı çıktı filan deme bug varsada fixle bide hazır burayı okutmuşken sana diyim yazdığım
}									  // şeyler 0 kontrol gidip derlemeyi dene hata alırsan fixle yada gemini e at genelde gpt den daha güzel bugfix yapıyo senin kararın tabi ama
									  // şuan githubun ekranından yazıyorum ve keyfi dikkatini çeksin diye kodyun yanına yazıyorum. BAK HATA ALIRSAN FİXLE


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

//THIS CODE IS A GIFT FROM THE BEST PROGRAMMER ON EARTH GLORIOUS KING MUSTAFA KAVAK.

void k_gets(char *buffer, unsigned int max_size) {
	unsigned int i = 0;
    unsigned char c;
	while (i < max_size - 1) {
        c = k_getch(); 
        if (c == '\n' || c == '\r') { 
            buffer[i] = '\0'; 
            k_printf("\n", CURRENT_LINE, WHITE); 
            break;
        } 
        else if (c == '\b' && i > 0) { 
            i--;
            // istersen  ekrandan sildirirsin burda şuan saat gece 2:56 ve gözüm acıyo o yüzzden sen yapcan
        } 
        else {
            buffer[i] = c;
            char temp[2] = {c, 0};
            k_printf(temp, CURRENT_LINE, WHITE);
            CURRENT_LINE--;
            i++;
        }
    }
    CURRENT_LINE++; 
}

//THIS CODE IS A GIFT FROM THE BEST PROGRAMMER ON EARTH GLORIOUS KING MUSTAFA KAVAK.

void k_main()  {
	k_clear_screen();
	k_printf("Hello, World!\n", CURRENT_LINE, WHITE_TXT); // \n burda çaluışıyomu gram fikrim yook

	char input[64];
    k_printf("vro enter something: ", CURRENT_LINE, WHITE);
    CURRENT_LINE--; // Input yazısı ile k_gets aynı satırda olsun diye
	k_gets(input, 64);

	k_printf("your text vro:", CURRENT_LINE, WHITE);
	k_printf(input, CURRENT_LINE, LIGHT_GREEN);
};
