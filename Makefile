# =============================================================================
# Variables
KERNEL_SIZE = 1024 

# Build tools

# =============================================================================
# Tasks

all: clean build test

.tmp/kernel.o: src/kernel.c
		gcc -std=c99 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector -c src/kernel.c -o .tmp/kernel.o

.tmp/boot.o: src/boot.asm
		nasm -felf -dKERNEL_SIZE=$(KERNEL_SIZE) src/boot.asm -o .tmp/boot.o

.tmp/os.elf: .tmp/boot.o .tmp/kernel.o link.ld
		ld -m elf_i386 .tmp/boot.o .tmp/kernel.o -T link.ld -o .tmp/os.elf

.tmp/os.bin: .tmp/os.elf
		objcopy -I elf32-i386 -O binary .tmp/os.elf .tmp/os.bin

os.img: .tmp/os.bin
		dd if=/dev/zero of=os.img bs=1024 count=1440
		dd if=.tmp/os.bin of=os.img conv=notrunc

build: os.img

clean:
		rm -f *.img
		rm -rf .tmp
		mkdir .tmp

test: build
		qemu-system-i386 -cpu pentium2 -m 1g -fda os.img -monitor stdio -device VGA

debug: build
		qemu-system-i386 -cpu pentium2 -m 1g -fda os.img -monitor stdio -device VGA -s -S &
		sleep 2
		gdb -x .gdbinit

.PHONY: all build clean test debug

