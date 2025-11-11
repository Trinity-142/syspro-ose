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
	init_printer();

	for (u32 i = 1; i; ++i) {
		u8* obj = malloc_undead(i*2, i);
		printf("%d byte object with %d byte alignment successfully allocated at address %x\n", i*2, i, (u32) obj);
	}
	endless_loop();
}