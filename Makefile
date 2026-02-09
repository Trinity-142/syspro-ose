# =============================================================================
# Variables
KERNEL_SIZE ?= 0x58400
DEBUG ?= 0
RAM_MB ?= 1024
EXP_NUM ?= 7 
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

# Stdlib =========================================================================
.tmp/userspace/%.o: src/stdlib/%.c
		gcc -DRAM_MB=$(RAM_MB) -DEXP_NUM=$(EXP_NUM) -DKERNEL_SIZE=$(KERNEL_SIZE) $(CFLAGS) -c $< -o $@

.tmp/userspace/%.asm.o: src/stdlib/%.asm
		nasm $(ASMFLAGS) -dKERNEL_SIZE=$(KERNEL_SIZE) $< -o $@

STDLIB_C_SRC = $(wildcard src/stdlib/*.c)
STDLIB_C_OBJ = $(patsubst src/stdlib/%.c, .tmp/userspace/%.o, $(STDLIB_C_SRC))
STDLIB_ASM_SRC = $(wildcard src/stdlib/*.asm)
STDLIB_ASM_OBJ = $(patsubst src/stdlib/%.asm, .tmp/userspace/%.asm.o, $(STDLIB_ASM_SRC))

# Userspace =========================================================================
APPS = $(notdir $(basename $(wildcard src/apps/*.c)))

SHARED_OBJS = $(STDLIB_C_OBJ) $(STDLIB_ASM_OBJ)

.tmp/userspace/%_app.o: src/apps/%.c
		gcc -DRAM_MB=$(RAM_MB) -DEXP_NUM=$(EXP_NUM) -DKERNEL_SIZE=$(KERNEL_SIZE) $(CFLAGS) -c $< -o $@

.tmp/userspace/%_app.elf: .tmp/userspace/%_app.o $(SHARED_OBJS)
		ld -m elf_i386 $^ -T src/stdlib/link.ld -o $@

.tmp/userspace/%_app.bin: .tmp/userspace/%_app.elf
		objcopy -I elf32-i386 -O binary $< $@

# All цели
APPS_ELF = $(addsuffix _app.elf, $(addprefix .tmp/userspace/,$(APPS)))
APPS_BIN = $(addsuffix _app.bin, $(addprefix .tmp/userspace/,$(APPS)))


# OS image ==========================================================================
.tmp/os.bin: .tmp/kernel/kernel.bin $(APPS_BIN)
		cp .tmp/kernel/kernel.bin $@
		truncate -s $$((0x18400)) $@
		cat .tmp/userspace/soe_app.bin >> $@
		truncate -s $$((0x28400)) $@
		cat .tmp/userspace/endless_inc_app.bin >> $@
		truncate -s $$((0x38400)) $@
		cat .tmp/userspace/foo_bar_baz_app.bin >> $@
		truncate -s $$((0x48400)) $@
		cat .tmp/userspace/factorial_app.bin >> $@

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

