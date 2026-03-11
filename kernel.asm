; 64-bit Long Mode Bootloader
bits 64
section .text
    align 4

    ; multiboot header (32-bit required for GRUB, but ok for loader)
    dd 0x1BADB002        ; magic
    dd 0x00               ; flags
    dd -(0x1BADB002 + 0x00) ; checksum

global start
extern k_main

start:
    cli                   ; disable interrupts

    ; Long mode setup (simplified, assumes GRUB loads kernel in 64-bit mode)
    jmp k_main             ; jump to kernel entry point

hlt_loop:
    hlt
    jmp hlt_loop