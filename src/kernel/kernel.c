#include "asm_utils.h"
#include "vga.h"
#include "userspace.h"

void kernel_entry() {
    vga_clear_screen();
    init_interrupts(INTERRUPT);
    set_interrupt_dpl(PRINT_CHAR_VECTOR, USER_PL);
    set_interrupt_dpl(EXIT_VECTOR, USER_PL);
    bool auto_eoi = true;
    pic8259_init_master(auto_eoi);
    pic8259_init_slave(auto_eoi);
    pic8259_turn(TIMER, true);
    sti();
    init_console(&consoles[0], (Coords){0,0}, (Coords){VGA_WIDTH / 2 - 1, VGA_HEIGHT / 2 - 1});
    init_console(&consoles[1], (Coords){VGA_WIDTH / 2,0}, (Coords){VGA_WIDTH / 2 - 1, VGA_HEIGHT / 2 - 1});
    init_console(&consoles[2], (Coords){0,VGA_HEIGHT / 2}, (Coords){VGA_WIDTH / 2 - 1, VGA_HEIGHT/ 2 - 1});
    init_console(&consoles[3], (Coords){VGA_WIDTH / 2,VGA_HEIGHT / 2}, (Coords){VGA_WIDTH / 2 - 1, VGA_HEIGHT / 2 - 1});
    //init_curr_process(0x20000, &consoles[0], 0);
    //current_process++;
    //init_curr_process(0x30000, &consoles[1], 0);
    //current_process++;
    init_curr_process(0x40000, &consoles[2], 3, "foo", "bar", "baz");
    /*
    current_process++;
    init_curr_process(0x50000, &consoles[3], 0);
    current_process = processes;
    */
    current_process = processes;
    jump_to_current_process();
    endless_loop();
}
