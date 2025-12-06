#include "alloc.h"
#include "asm_utils.h"
#include "vga.h"

void user_main() {
	endless_loop();
}

void kernel_entry() {
	vga_clear_screen();
	enter_userspace(user_main, malloc_undead(4096, 16) + 4096);
	endless_loop();
}
