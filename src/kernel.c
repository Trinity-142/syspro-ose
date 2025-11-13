#include "asm_utils.h"
#include "types.h"
#include "vga.h"
#include "interrupts.h"
#include "printf.h"
#include "experiments.h"

void delay() {
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 50000; ++j) {
			write_u8(0x80, 0x80);
		}
		printf("%d ", i);
	}
	printf("\n");
}

u32 global = 42;
u32 N = 52;

static void timer_handler(Context *ctx) {
	TIMER_HANDLER(EXP_NUM);
}

static void keyboard_handler(Context *ctx) {
	KEYBOARD_HANDLER(EXP_NUM);
}

void kernel_entry() {
	vga_clear_screen();
	EXPERIMENT(EXP_NUM);
}
