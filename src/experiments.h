#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H
#define EXPERIMENT(n) EXP(n)
#define EXP(n) EXP_##n
#define TIMER_HANDLER(n) TIMER(n)
#define TIMER(n) TIMER_HANDLER_##n
#define USERSPACE_PROCESS(n) USER(n)
#define USER(n) USERSPACE_PROCESS_##n

#define EXP_1                                           \
    vga_clear_screen();                                 \
    init_interrupts(INTERRUPT);                         \
    bool auto_eoi = true;                               \
    pic8259_init_master(auto_eoi);                      \
    pic8259_init_slave(auto_eoi);                       \
    enter_userspace(user_main, malloc_undead(4096, 16) + 4096);
#define TIMER_HANDLER_1
#define USERSPACE_PROCESS_1                             \
    endless_loop();

#define EXP_2 EXP_1
#define TIMER_HANDLER_2
#define USERSPACE_PROCESS_2                             \
    printf("Hello from Userspace!");                    \
    endless_loop();

#define EXP_3 EXP_1
#define TIMER_HANDLER_3
#define USERSPACE_PROCESS_3                             \
    for(;;) { \
        printf("%d ", global);                          \
        global++;                                       \
    }

#define EXP_4 EXP_1
#define TIMER_HANDLER_4
#define USERSPACE_PROCESS_4                             \
    printf("%x ", get_esp());                           \
    endless_loop();

#define EXP_5 EXP_1
#define TIMER_HANDLER_5
#define USERSPACE_PROCESS_5                             \
    cli();                                              \
//    in();                                               \
//    lgdt();                                             \
//    lidt();                                             \
//    ltr();                                              \
//    mov cr0();                                          \
    endless_loop();

#define EXP_6                                           \
    vga_clear_screen();                                 \
    init_interrupts(INTERRUPT);                         \
    bool auto_eoi = true;                               \
    pic8259_init_master(auto_eoi);                      \
    pic8259_init_slave(auto_eoi);                       \
    pic8259_turn(TIMER, true);                          \
    enter_userspace(user_main, malloc_undead(4096, 16) + 4096); \
    sti();                                              \
	enter_userspace(user_main, malloc_undead(4096, 16) + 4096);
#define TIMER_HANDLER_6                                 \
    printf("%x ", ctx->esp);                            \
    endless_loop();
#define USERSPACE_PROCESS_6                             \
    for(;;) {                                           \
        printf("%d ", global);                          \
        global++;                                       \
    }

#define EXP_7 EXP_6
#define TIMER_HANDLER_7                                 \
    global = 0;
#define USERSPACE_PROCESS_7 USERSPACE_PROCESS_6

#define EXP_8 EXP_7
#define TIMER_HANDLER_8                                 \
    global = 0;                                         \
    printf("\n%d\n", timer++);
#define USERSPACE_PROCESS_8 USERSPACE_PROCESS_7

#define EXP_9 EXP_6
#define TIMER_HANDLER_9
#define USERSPACE_PROCESS_9                             \
    extern u8 gdt_kernel_code[];                        \
	volatile u8* fifth_byte = &gdt_kernel_code[5];      \
	*fifth_byte = *fifth_byte & 0x7F;                   \
    endless_loop();

#define EXP_10 EXP_6
#define TIMER_HANDLER_10                                \
    global = 0;
#define USERSPACE_PROCESS_10                            \
    for(;;) {                                           \
        write(global);                                  \
        global++;                                       \
    }

#endif
