#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#define APPS(n) APP(n)
#define APP(n) APPS_##n

#define TIMER_HANDLER(n) TIMER(n)
#define TIMER(n) TIMER_HANDLER_##n

#define EXIT_HANDLER(n) EXIT(n)
#define EXIT(n) EXIT_HANDLER_##n

#define PAGE_FAULT_HANDLER(n) PF(n)
#define PF(n) PAGE_FAULT_HANDLER_##n

#define EXP_SETUP                                       															\
	vga_clear_screen();																								\
    init_interrupts(INTERRUPT);																						\
    set_interrupt_dpl(PRINT_CHAR_VECTOR, USER_PL);																	\
    set_interrupt_dpl(EXIT_VECTOR, USER_PL);																		\
    bool auto_eoi = true;																							\
    pic8259_init_master(auto_eoi);																					\
    pic8259_init_slave(auto_eoi);																					\
    pic8259_turn(TIMER, true);																						\
    sti();																											\
    init_console(&consoles[0], (Coords){0,0}, (Coords){VGA_WIDTH / 2 - 1, VGA_HEIGHT / 2 - 1});						\
    init_console(&consoles[1], (Coords){VGA_WIDTH / 2,0}, (Coords){VGA_WIDTH / 2 - 1, VGA_HEIGHT / 2 - 1});			\
    init_console(&consoles[2], (Coords){0,VGA_HEIGHT / 2}, (Coords){VGA_WIDTH / 2 - 1, VGA_HEIGHT/ 2 - 1});			\
    init_console(&consoles[3], (Coords){VGA_WIDTH / 2,VGA_HEIGHT / 2}, (Coords){VGA_WIDTH / 2 - 1, VGA_HEIGHT / 2 - 1});
//================================================================================================================== 1
#define APPS_1																									    \
	endless_loop();
#define TIMER_HANDLER_1
#define EXIT_HANDLER_1
//================================================================================================================== 2
#define APPS_2																									    \
    current_process = processes;																					\
	init_curr_process(0x20000, &consoles[0], 0);																	\
	jump_to_current_process();																						\
	endless_loop();
#define TIMER_HANDLER_2
#define EXIT_HANDLER_2																								\
	printf("---------\n");																							\
    printf("process exited with code: %d\n", ctx->eax);																\
    turn_paging_off();																								\
    cleanup_process();																								\
    current_process->terminated = true;																				\
    assert(kalloc == kfree);																						\
    endless_loop();
//================================================================================================================== 3
#define APPS_3																									    \
    current_process = processes;																					\
	init_curr_process(0x40000, &consoles[0], 3, "foo", "bar", "baz");																	\
	jump_to_current_process();																						\
	endless_loop();
#define TIMER_HANDLER_3
#define EXIT_HANDLER_3																								\
	printf("---------\n");																							\
    printf("process exited with code: %d\n", ctx->eax);																\
    turn_paging_off();																								\
    cleanup_process();																								\
    current_process->terminated = true;																				\
    assert(kalloc == kfree);																						\
    endless_loop();
//================================================================================================================== 4
#define APPS_4																									    \
    current_process = processes;																					\
	init_curr_process(0x30000, &consoles[0], 0);																	\
	jump_to_current_process();																						\
	endless_loop();
#define TIMER_HANDLER_4
#define EXIT_HANDLER_4																								\
	printf("---------\n");																							\
    printf("process exited with code: %d\n", ctx->eax);																\
    turn_paging_off();																								\
    cleanup_process();																								\
    current_process->terminated = true;																				\
    assert(kalloc == kfree);																						\
    endless_loop();
//================================================================================================================== 5
#define APPS_5																									    \
    current_process = processes;																					\
	init_curr_process(0x20000, &consoles[0], 0);																	\
	jump_to_current_process();																						\
	endless_loop();
#define TIMER_HANDLER_5
#define EXIT_HANDLER_5																								\
	printf("---------\n");																							\
    printf("process exited with code: %d\n", ctx->eax);																\
    turn_paging_off();																								\
    cleanup_process();																								\
    current_process->terminated = true;																				\
    assert(kalloc == kfree);																						\
    init_curr_process(0x20000, &consoles[0], 0);																	\
    jump_to_current_process();
//================================================================================================================== 6
#define APPS_6 APPS_5
#define TIMER_HANDLER_6 TIMER_HANDLER_5
#define EXIT_HANDLER_6 EXIT_HANDLER_5
//================================================================================================================== 7
#define APPS_7																									    \
    current_process = processes;																					\
	init_curr_process(0x20000, &consoles[0], 0);																	\
	current_process++;																								\
	init_curr_process(0x30000, &consoles[1], 0);																	\
	current_process++;																								\
	init_curr_process(0x40000, &consoles[2], 3, "foo", "bar", "baz");												\
	current_process++;																								\
	init_curr_process(0x50000, &consoles[3], 0);																	\
    current_process = processes;																					\
	jump_to_current_process();																						\
	endless_loop();
#define TIMER_HANDLER_7																								\
    current_process->ctx = *ctx;																					\
    jump_to_next_process();
#define EXIT_HANDLER_7																								\
	printf("---------\n");																							\
    printf("process exited with code: %d\n", ctx->eax);																\
    turn_paging_off();																								\
    cleanup_process();																								\
    current_process->terminated = true;																				\
    assert(kalloc == kfree);																						\
    jump_to_next_process();
#endif
