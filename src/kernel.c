#include "printf.h"
#include "alloc.h"
#include "asm_utils.h"
#include "experiments.h"
#include "vga.h"

void user_main() {
	USERSPACE_PROCESS(EXP_NUM);
}

void kernel_entry() {
	EXPERIMENT(EXP_NUM);
}
