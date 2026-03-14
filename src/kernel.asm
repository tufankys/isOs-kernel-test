[BITS 64]

global _start
extern kernel_main

_start:
    mov rsp, 0x90000      ; 16-byte hizalı bir adres
    and rsp, -16          ; Son 4 biti sıfırlayarak 16-byte hizalamayı garanti et
    call kernel_main
    jmp $
.halt:
    hlt
    jmp .halt