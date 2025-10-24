# =============================================================================
# Variables
KERNEL_SIZE ?= 4096
DEBUG ?= 0
# Build tools
ifeq ($(DEBUG),1)
	CFLAGS = -DDEBUG -std=c99 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector
else
	CFLAGS = -std=c99 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector
endif
# =============================================================================
# Tasks

all: clean build test

.tmp/%.o: src/%.c
		gcc -DKERNEL_SIZE=$(KERNEL_SIZE) $(CFLAGS) -c $< -o $@

.tmp/%.o: src/%.asm
		nasm -felf -dKERNEL_SIZE=$(KERNEL_SIZE) $< -o $@

C_SOURCES = $(wildcard src/*.c)
C_OBJECTS = $(patsubst src/%.c, .tmp/%.o, $(C_SOURCES))
ASM_SOURCES = $(wildcard src/*.asm)
ASM_OBJECTS = $(patsubst src/%.asm, .tmp/%.o, $(ASM_SOURCES))
.tmp/os.elf: $(ASM_OBJECTS) $(C_OBJECTS) link.ld
		ld -m elf_i386 $(ASM_OBJECTS) $(C_OBJECTS) -T link.ld -o .tmp/os.elf

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

