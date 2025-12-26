# =============================================================================
# Variables
KERNEL_SIZE ?= 0x58400
DEBUG ?= 0
RAM_MB ?= 1024
EXP_NUM ?= 1
# Build tools
ifeq ($(DEBUG),1)
	CFLAGS = -DDEBUG -g -std=c99 -m32 -O0 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector
	ASMFLAGS = -felf -g
else
	CFLAGS = -std=c99 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector
	ASMFLAGS = -felf
endif

all: clean build test

# Kernel =============================================================================
.tmp/kernel/%.o: src/kernel/%.c
		gcc -DRAM_MB=$(RAM_MB) -DEXP_NUM=$(EXP_NUM) -DKERNEL_SIZE=$(KERNEL_SIZE) $(CFLAGS) -c $< -o $@

.tmp/kernel/%.o: src/kernel/%.asm
		nasm $(ASMFLAGS) -dKERNEL_SIZE=$(KERNEL_SIZE) $< -o $@

C_SOURCES = $(wildcard src/kernel/*.c)
C_OBJECTS = $(patsubst src/kernel/%.c, .tmp/kernel/%.o, $(C_SOURCES))
ASM_SOURCES = $(wildcard src/kernel/*.asm)
ASM_OBJECTS = $(patsubst src/kernel/%.asm, .tmp/kernel/%.o, $(ASM_SOURCES))

.tmp/kernel/kernel.elf: $(C_OBJECTS) $(ASM_OBJECTS) src/kernel/link.ld
		ld -m elf_i386 $(C_OBJECTS) $(ASM_OBJECTS) -T src/kernel/link.ld -o .tmp/kernel/kernel.elf

.tmp/kernel/kernel.bin: .tmp/kernel/kernel.elf
		objcopy -I elf32-i386 -O binary .tmp/kernel/kernel.elf .tmp/kernel/kernel.bin
		@SIZE=$$(stat -c%s .tmp/kernel/kernel.bin); \
		if [ $$SIZE -gt $(KERNEL_SIZE) ]; then \
			echo "Error: Kernel size $$SIZE exceeds limit $(KERNEL_SIZE)"; \
			exit 1; \
	  	fi

# Userspace =========================================================================
.tmp/userspace/%.o: src/stdlib/%.c
		gcc -DRAM_MB=$(RAM_MB) -DEXP_NUM=$(EXP_NUM) -DKERNEL_SIZE=$(KERNEL_SIZE) $(CFLAGS) -c $< -o $@

.tmp/userspace/%.asm.o: src/stdlib/%.asm
		nasm $(ASMFLAGS) -dKERNEL_SIZE=$(KERNEL_SIZE) $< -o $@

.tmp/userspace/%.o: src/apps/%.c
		gcc -DRAM_MB=$(RAM_MB) -DEXP_NUM=$(EXP_NUM) -DKERNEL_SIZE=$(KERNEL_SIZE) $(CFLAGS) -c $< -o $@

.tmp/userspace/%.o: src/apps/%.asm
		nasm $(ASMFLAGS) -dKERNEL_SIZE=$(KERNEL_SIZE) $< -o $@


STDLIB_C_SRC = $(wildcard src/stdlib/*.c)
STDLIB_C_OBJ = $(patsubst src/stdlib/%.c, .tmp/userspace/%.o, $(STDLIB_C_SRC))
STDLIB_ASM_SRC = $(wildcard src/stdlib/*.asm)
STDLIB_ASM_OBJ = $(patsubst src/stdlib/%.asm, .tmp/userspace/%.asm.o, $(STDLIB_ASM_SRC))

APPS_C_SRC = $(wildcard src/apps/*.c)
APPS_C_OBJ = $(patsubst src/apps/%.c, .tmp/userspace/%.o, $(APPS_C_SRC))
APPS_ASM_SRC = $(wildcard src/apps/*.asm)
APPS_ASM_OBJ = $(patsubst src/apps/%.asm, .tmp/userspace/%.o, $(APPS_ASM_SRC))

C_OBJECTS_USERSPACE = $(STDLIB_C_OBJ) $(APPS_C_OBJ)
ASM_OBJECTS_USERSPACE = $(STDLIB_ASM_OBJ) $(APPS_ASM_OBJ)

.tmp/userspace/userspace.elf: $(C_OBJECTS_USERSPACE) $(ASM_OBJECTS_USERSPACE) src/stdlib/link.ld
		ld -m elf_i386 $(C_OBJECTS_USERSPACE) $(ASM_OBJECTS_USERSPACE) -T src/stdlib/link.ld -o .tmp/userspace/userspace.elf

.tmp/userspace/userspace.bin: .tmp/userspace/userspace.elf
		objcopy -I elf32-i386 -O binary .tmp/userspace/userspace.elf .tmp/userspace/userspace.bin

# OS image ==========================================================================
.tmp/os.bin: .tmp/kernel/kernel.bin .tmp/userspace/userspace.bin
	cp .tmp/kernel/kernel.bin $@
	truncate -s $$((0x18400)) $@
	cat .tmp/userspace/userspace.bin >> $@

os.img:	.tmp/os.bin
		dd if=/dev/zero of=os.img bs=1024 count=1440
		dd if=.tmp/os.bin of=os.img conv=notrunc

build: os.img

clean:
		rm -f *.img
		rm -rf .tmp
		mkdir -p .tmp/kernel
		mkdir -p .tmp/userspace

test: build
		qemu-system-i386 -cpu pentium2 -m $(RAM_MB)m -fda os.img -monitor stdio -device VGA

debug: clean build
		qemu-system-i386 -cpu pentium2 -m $(RAM_MB)m -fda os.img -monitor stdio -device VGA -s -S &
		#sleep 2
		#gdb -x .gdbinit

.PHONY: all build clean test debug

