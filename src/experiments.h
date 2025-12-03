#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H
#include "pic8259.h"
#define EXPERIMENT(n) EXP(n)
#define EXP(n) EXP_##n
#define TIMER_HANDLER(n) TIMER(n)
#define TIMER(n) TIMER_HANDLER_##n
#define KEYBOARD_HANDLER(n) KEYBOARD(n)
#define KEYBOARD(n) KEYBOARD_HANDLER_##n

#define EXP_1                                           \
    init_interrupts(INTERRUPT);                         \
    bool auto_eoi = false;                              \
    pic8259_init_master(auto_eoi);                      \
    pic8259_init_slave(auto_eoi);                       \
    sti();                                              \
    endless_loop();

#define EXP_2                                           \
    init_interrupts(INTERRUPT);                         \
    bool auto_eoi = false;                              \
    pic8259_init_master(auto_eoi);                     \
    pic8259_init_slave(auto_eoi);                      \
    pic8259_turn(TIMER, true);   \
    sti();                                              \
    endless_loop();

#define EXP_3                                           \
    init_interrupts(INTERRUPT);                         \
    bool auto_eoi = false;                              \
    pic8259_init_master(auto_eoi);                      \
    pic8259_init_slave(auto_eoi);                       \
    pic8259_turn(KEYBOARD, true);                       \
    sti();                                              \
    endless_loop();

#define EXP_4                                           \
    init_interrupts(INTERRUPT);                         \
    bool auto_eoi = true;                               \
    pic8259_init_master(auto_eoi);                     \
    pic8259_init_slave(auto_eoi);                      \
    pic8259_turn(TIMER, true);       \
    sti();                                              \
    endless_loop();

#define EXP_5                                           \
    init_interrupts(INTERRUPT);                         \
    bool auto_eoi = true;                               \
    pic8259_init_master(auto_eoi);                     \
    pic8259_init_slave(auto_eoi);                      \
    pic8259_turn(TIMER, true);       \
    sti();                                              \
    for (;;) printf("%d ", global++);

#define EXP_6 EXP_5

#define EXP_7                                           \
    init_interrupts(INTERRUPT);                         \
    bool auto_eoi = false;                              \
    pic8259_init_master(auto_eoi);                     \
    pic8259_init_slave(auto_eoi);                      \
    pic8259_turn(TIMER, true);       \
    sti();                                              \
    endless_loop();

#define EXP_8 EXP_7
#define EXP_9 EXP_7
#define EXP_10 EXP_7

#define EXP_11                                          \
    init_interrupts(TRAP);                              \
    bool auto_eoi = false;                              \
    pic8259_init_master(auto_eoi);                     \
    pic8259_init_slave(auto_eoi);                      \
    pic8259_turn(TIMER, true);       \
    sti();                                              \
    endless_loop();

#define EXP_12                                          \
    init_interrupts(INTERRUPT);                         \
    bool auto_eoi = true;                               \
    pic8259_init_master(auto_eoi);                     \
    pic8259_init_slave(auto_eoi);                      \
    pic8259_turn(TIMER, true);       \
    sti();                                              \
    endless_loop();

#define EXP_13                                          \
    init_interrupts(INTERRUPT);                         \
    bool auto_eoi = true;                               \
    pic8259_init_master(auto_eoi);                     \
    pic8259_init_slave(auto_eoi);                      \
    pic8259_turn(KEYBOARD, true); \
    sti();                                              \
    endless_loop();

#define EXP_14                                          \
    init_interrupts(INTERRUPT);                         \
    bool auto_eoi = true;                               \
    pic8259_init_master(auto_eoi);                     \
    pic8259_init_slave(auto_eoi);                      \
    pic8259_turn(TIMER, true);       \
    pic8259_turn(KEYBOARD, true); \
    sti();                                              \
    endless_loop();

#define EXP_15 EXP_14

#define EXP_16 EXP_14

#define EXP_17 EXP_14

#define EXP_18                                          \
    init_interrupts(TRAP);                              \
    bool auto_eoi = false;                              \
    pic8259_init_master(auto_eoi);                     \
    pic8259_init_slave(auto_eoi);                      \
    pic8259_turn(TIMER, true);       \
    pic8259_turn(KEYBOARD, true); \
    sti();                                              \
    endless_loop();

#define EXP_19 EXP_18

#define EXP_20                                          \
    init_interrupts(INTERRUPT);                         \
    bool auto_eoi = true;                               \
    pic8259_init_master(auto_eoi);                     \
    pic8259_init_slave(auto_eoi);                      \
    pic8259_turn(TIMER, true);       \
    pic8259_turn(KEYBOARD, true); \
    sti();                                              \
    endless_loop();






#define TIMER_HANDLER_1
#define KEYBOARD_HANDLER_1

#define TIMER_HANDLER_2
#define KEYBOARD_HANDLER_2

#define TIMER_HANDLER_3
#define KEYBOARD_HANDLER_3                              \
    print_context(ctx);

#define TIMER_HANDLER_4                                 \
    printf("%d ", global++);
#define KEYBOARD_HANDLER_4

#define TIMER_HANDLER_5                                 \
    global = 0;
#define KEYBOARD_HANDLER_5

#define TIMER_HANDLER_6                                 \
    printf("%d ", global++);
#define KEYBOARD_HANDLER_6

#define TIMER_HANDLER_7                                 \
    printf("%d ", global++);
#define KEYBOARD_HANDLER_7

#define TIMER_HANDLER_8                                 \
    printf("%d ", global++);                            \
    pic8259_send_EOI(TIMER);
#define KEYBOARD_HANDLER_8

#define TIMER_HANDLER_9                                 \
    printf("%d ", global++);                            \
    pic8259_send_EOI(TIMER);                            \
    sti();                                              \
    endless_loop();
#define KEYBOARD_HANDLER_9

#define TIMER_HANDLER_10                                \
    printf("%d ", global++);                            \
    if (global < N) {                                   \
        pic8259_send_EOI(TIMER);                        \
        sti();                                          \
    }                                                   \
    endless_loop();
#define KEYBOARD_HANDLER_10

#define TIMER_HANDLER_11                                \
    printf("%d ", global++);                            \
    u32 N = 52;                                         \
    if (global < N) {                                   \
        pic8259_send_EOI(TIMER);                        \
    }                                                   \
    endless_loop();
#define KEYBOARD_HANDLER_11

#define TIMER_HANDLER_12                                \
    printf("%d ", global++);                            \
    u32 N = 52;                                         \
    if (global < N) {                                   \
        sti();                                          \
    }                                                   \
    endless_loop();
#define KEYBOARD_HANDLER_12

#define TIMER_HANDLER_13
#define KEYBOARD_HANDLER_13                             \
    printf("%x ", read_u8(0x60));

#define TIMER_HANDLER_14                                \
    printf("%d ", global++);
#define KEYBOARD_HANDLER_14                             \
    printf("%x ", read_u8(0x60));

#define TIMER_HANDLER_15                                \
    printf("%d ", global++);                            \
    endless_loop();
#define KEYBOARD_HANDLER_15                             \
   printf("%x ", read_u8(0x60));

#define TIMER_HANDLER_16                                \
    printf("%d ", global++);
#define KEYBOARD_HANDLER_16                             \
    printf("%x ", read_u8(0x60));                       \
    endless_loop();

#define TIMER_HANDLER_17                                \
    printf("%d ", global++);
#define KEYBOARD_HANDLER_17                             \
    printf("%x ", read_u8(0x60));                       \
    sti();                                              \
    endless_loop();

#define TIMER_HANDLER_18                                \
    printf("%d ", global++);                            \
    endless_loop();
#define KEYBOARD_HANDLER_18                             \
    printf("%x ", read_u8(0x60));                       \
    pic8259_send_EOI(KEYBOARD);                         \
    endless_loop();

#define TIMER_HANDLER_19                                \
    printf("%d ", global++);                            \
    pic8259_send_EOI(TIMER);
#define KEYBOARD_HANDLER_19                             \
    printf("%x ", read_u8(0x60));                       \
    endless_loop();

#define TIMER_HANDLER_20                                \
    pic8259_turn(TIMER, false);                     \
    delay();                                            \
    sti();                                              \
    delay();
#define KEYBOARD_HANDLER_20                             \
    printf("%x ", read_u8(0x60));                       \
    endless_loop();


#endif EXPERIMENTS_H