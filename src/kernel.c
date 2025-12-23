#include "printf.h"
#include "alloc.h"
#include "asm_utils.h"
#include "experiments.h"
#include "vga.h"
#include "allocator.h"
#include "userspace.h"

void kernel_entry() {
	vga_clear_screen();
	init_interrupts(INTERRUPT);
	bool auto_eoi = true;
	pic8259_init_master(auto_eoi);
	pic8259_init_slave(auto_eoi);
	pic8259_turn(TIMER, true);
	sti();
	printf("%d %x", RAM_MB, RAM);
	init_allocator((u8*) POOL_START, (u8*) RAM, PAGE);
	init_paging();
	printf("Paging enabled\n");
	printf("%x\n", RAM);
	param = 0;
	start_usercode();
}
