#include "asm_utils.h"
#include "types.h"
#include "vga.h"
#include "alloc.h"
#include "assert.h"
#include "interrupts.h"
#include "printf.h"

void kernel_entry() {
	vga_clear_screen();
	init_interrupts();
	set_gprs();
	division_by_zero();
	//syscall();
	//sti();
	endless_loop();
}
