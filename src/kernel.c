#include "asm_utils.h"
#include "types.h"
#include "vga.h"
#include "alloc.h"
#include "printf.h"

void kernel_entry() {
	init_printer();

	for (u32 i = 1; i; ++i) {
		u8* obj = malloc_undead(i*2, i);
		printf("%d byte object with %d byte alignment successfully allocated at address %x\n", i*2, i, (u32) obj, (u32) obj);
	}

	for (u32 i = 1; i; ++i) {
		printf("%d ", i);
	}
	endless_loop();
}