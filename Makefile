# =============================================================================
# Variables

# Build tools
NASM = nasm -f bin 
N = 512 
DUMP_SIZE = $(shell echo $$(($(N) + 512)))

# =============================================================================
# Tasks

all: clean build test

.tmp/boot.bin: src/boot.asm
	$(NASM) src/boot.asm -o .tmp/boot.bin

boot.img: .tmp/boot.bin
	dd if=/dev/zero of=boot.img bs=1024 count=1440
	dd if=.tmp/boot.bin of=boot.img conv=notrunc
	dd if=foo of=boot.img conv=notrunc seek=1

build: boot.img

clean:
	rm -f *.img
	rm -rf .tmp
	mkdir .tmp

test: build
		
	echo "pmemsave 0x7C00 $(DUMP_SIZE) bar" > qemu_commands	
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA < qemu_commands
	hexdump -C bar
	hexdump -C boot.img	

debug: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA -s -S &
	gdb

.PHONY: all build clean test debug
