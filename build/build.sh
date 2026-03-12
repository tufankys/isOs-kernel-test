nasm -f elf32 src/kernel.asm -o build/output/kasm.o
gcc -m32 -ffreestanding -fno-builtin -fno-stack-protector -c src/kernel.c -o build/output/kc.o
ld -m elf_i386 -T build/link.ld -o build/output/kernel build/output/kasm.o build/output/kc.o
qemu-system-i386 -kernel build/output/kernel