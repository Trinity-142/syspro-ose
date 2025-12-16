#include <stdbool.h>

#include "asm_utils.h"
#include "vga.h"
#include "interrupts.h"
#include "pic8259.h"
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

void kernel_entry() {
	vga_clear_screen();
	EXPERIMENT(EXP_NUM);
}
