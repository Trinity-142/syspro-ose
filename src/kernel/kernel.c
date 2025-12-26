#include "asm_utils.h"
#include "experiments.h"
#include "vga.h"
#include "userspace.h"

void kernel_entry() {
    init_paging();
    vga_clear_screen();
    init_interrupts(INTERRUPT);
    set_interrupt_dpl(PRINT_CHAR_VECTOR, USER_PL);
    set_interrupt_dpl(EXIT_VECTOR, USER_PL);
    bool auto_eoi = true;
    pic8259_init_master(auto_eoi);
    pic8259_init_slave(auto_eoi);
    pic8259_turn(TIMER, true);
    sti();
    //endless_loop();
    start_usercode();
    endless_loop();
}
