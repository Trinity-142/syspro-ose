#include "asm_utils.h"
#include "experiments.h"
#include "panic.h"
#include "vga.h"
#include "userspace.h"

void kernel_entry() {
    init_pd();
    vga_clear_screen();
    init_interrupts(INTERRUPT);
    set_interrupt_dpl(PRINT_CHAR_VECTOR, USER_PL);
    set_interrupt_dpl(EXIT_VECTOR, USER_PL);
    bool auto_eoi = true;
    pic8259_init_master(auto_eoi);
    pic8259_init_slave(auto_eoi);
    pic8259_turn(TIMER, true);
    sti();
    init_console(&consoles[0], (Coords){0,0}, (Coords){40, 12});
//    init_curr_process(0x20000, &consoles[0], 3, "foo", "bar", "baz");
    init_curr_process(0x20000, &consoles[0], 0);
    start_usercode();
    endless_loop();
}
