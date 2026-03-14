FILES = ./build/kernel.asm.o ./build/kernel.o
FLAGS = -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all:
	nasm -f bin ./src/boot.asm -o ./bin/boot.bin
	nasm -f elf64 ./src/kernel.asm -o ./build/kernel.asm.o

	gcc -m64 -I./src -ffreestanding -fno-stack-protector -fno-pie -c ./src/kernel.c -o ./build/kernel.o

	ld -m elf_x86_64 -T ./src/linkerScript.ld -o ./bin/kernel.bin \
   ./build/kernel.asm.o \
   ./build/kernel.o \
   --oformat binary

	dd if=./bin/boot.bin of=./bin/os.bin bs=512 count=1
	dd if=./bin/kernel.bin of=./bin/os.bin bs=512 seek=1 conv=notrunc
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

	qemu-system-x86_64 -drive format=raw,file=./bin/os.bin -d int

clean:
	rm -f ./bin/boot.bin
	rm -f ./bin/kernel.bin
	rm -f ./bin/os.bin
	rm -f ./build/kernel.asm.o
	rm -f ./build/kernel.o
	rm -f ./build/completeKernel.o