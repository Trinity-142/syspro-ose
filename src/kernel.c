#include "asm_utils.h"
#include "types.h"
#include "vga.h"
#include "alloc.h"
#include "assert.h"
#include "interrupts.h"
#include "printf.h"

u32 a = 42;
static void timer_handler(Context* ctx) {
	//a = 0;
	printf("%d ", a++);

//	if (a < 52) {
//		pic8259_send_EOI(TIMER);
//		sti();
//	}
//	endless_loop();
}

static void keyboard_handler(Context* ctx) {
	printf("%x ", read_pic8259(0x60));
	//pic8259_send_EOI(KEYBOARD);
	//endless_loop();
}


void kernel_entry() {
	vga_clear_screen();
	init_interrupts(INTERRUPT);
	printf("Hllo World\n");
	bool auto_eoi = true;
	pic8259_init(MASTER, auto_eoi);
	pic8259_init(SLAVE, auto_eoi);
	pic8259_enable_device(TIMER, timer_handler);
	pic8259_enable_device(KEYBOARD, keyboard_handler);
	sti();
	endless_loop();
	//for (;;) printf("%d ", a++);
}
