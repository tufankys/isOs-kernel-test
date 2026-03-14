[BITS 16]
[ORG 0x7c00]

; --- Sabitler ---
KERNEL_START_ADDR equ 0x100000

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    ; 1. Kerneli diskten yükle (15 sektör ~7.5KB okur, yetmezse sayıyı artır)
    mov bx, 0x1000       ; KERNEL_LOAD_SEG
    mov es, bx
    mov bx, 0x0
    mov ah, 0x02
    mov al, 30           ; Sektör sayısı (Kernel büyürse burayı artır)
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02         ; 2. sektörden başla
    mov dl, 0x80         ; İlk HDD
    int 0x13
    jc disk_error

    ; 2. 32-bit Protected Mode'a Geçiş Hazırlığı
    cli
    lgdt [gdt32_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:PModeMain

disk_error:
    hlt

; --- 32-bit GDT ---
gdt32_start:
    dq 0x0
    dq 0x00cf9a000000ffff ; Code
    dq 0x00cf92000000ffff ; Data
gdt32_end:
gdt32_descriptor:
    dw gdt32_end - gdt32_start - 1
    dd gdt32_start

[BITS 32]
PModeMain:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x90000

    ; 3. 64-bit Paging Hazırlığı (2MB Huge Pages)
    ; Belleği temizle (0x1000 - 0x4000 arası)
    mov edi, 0x1000
    xor eax, eax
    mov ecx, 4096
    rep stosd

    ; Tabloları birbirine bağla
    mov edi, 0x1000
    mov cr3, edi         ; PML4 adresini CR3'e bildir
    mov dword [edi], 0x2003      ; PML4[0] -> PDPT (0x2000)
    
    mov edi, 0x2000
    mov dword [edi], 0x3003      ; PDPT[0] -> Page Directory (0x3000)

    ; Page Directory (PD) doldurma - İlk 64MB'ı haritala
    ; Her giriş 2MB'lık bir "Huge Page" (0x83) temsil eder
    mov edi, 0x3000
    mov ebx, 0x00000083          ; Present + Writable + PS (Bit 7)
    mov ecx, 32                  ; 32 giriş * 2MB = 64MB alan
.fill_pd:
    mov [edi], ebx
    add ebx, 0x200000            ; Adresi 2MB ileri taşı
    add edi, 8                   ; Bir sonraki tablo girişi (64-bit girişler 8 byte'tır)
    loop .fill_pd

    ; 4. Long Mode Aktif Etme
    mov eax, cr4
    or eax, 1 << 5               ; PAE aktif
    mov cr4, eax

    mov ecx, 0xC0000080          ; EFER MSR
    rdmsr
    or eax, 1 << 8               ; LME (Long Mode Enable)
    wrmsr

    mov eax, cr0
    or eax, 1 << 31              ; Paging aktif
    mov cr0, eax

    ; 5. 64-bit GDT'ye Geçiş
    lgdt [gdt64_descriptor]
    jmp 0x08:LongModeMain

; --- 64-bit GDT ---
gdt64_start:
    dq 0x0
    dq 0x00af9a000000ffff ; 64-bit Code (L-bit set)
    dq 0x00af92000000ffff ; 64-bit Data
gdt64_end:
gdt64_descriptor:
    dw gdt64_end - gdt64_start - 1
    dq gdt64_start

[BITS 64]
LongModeMain:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov rsp, 0x90000             ; Stack'i 64-bit için tekrar kur

    ; Real Mode'da 0x1000:0x0000 (0x10000) adresine yüklenen 
    ; kernel'ı 1MB (0x100000) adresine taşıyoruz.
    mov rsi, 0x10000             ; Kaynak
    mov rdi, KERNEL_START_ADDR    ; Hedef
    mov rcx, 1024                ; Kopyalanacak miktar (Artırılabilir)
    rep movsq                    ; 64-bit bloklar halinde taşı

    ; Ve nihayet C Dünyası!
    jmp KERNEL_START_ADDR

times 510-($-$$) db 0
dw 0xaa55