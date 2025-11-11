#include "asm_utils.h"
#include "types.h"
#include "vga.h"
#include "alloc.h"
#include "printf.h"

void kernel_entry() {
	init_printer();
    printf("kernel size = %d\n", KERNEL_SIZE);
    printf("0x7C00 + kernel size = %x\n", 0x7c00 + KERNEL_SIZE);
	for (u32 i = 1; ; ++i) {
		u8* obj = malloc_undead(i*2, i);
		printf("%d byte object with %d byte alignment successfully allocated at address %x\n", i*2, i, (u32) obj);
	}
	endless_loop();
}
