#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include <stdalign.h>
#include <stdbool.h>

#include "types.h"

#define TRAP_GATE 0b111
#define TRAMPOLINE_SIZE 8
#define IRQs 16
#define IRQ0_VECTOR 0x20
#define IRQ8_VECTOR 0x28
#define IRQ15_VECTOR 0x
#define SLAVE_IRQ 2

typedef enum {
    MASTER,
    SLAVE
} PIC8259Type;

typedef enum {
    MASTER_COMMAND = 0x20,
    MASTER_DATA = 0x21,
    SLAVE_COMMAND = 0xA0,
    SLAVE_DATA = 0xA1,
    DELAY = 0x80
} Port;

typedef enum {
    INTERRUPT = 0b110,
    TRAP = 0b111
} GateType;

typedef enum {
    TIMER = 0,
    KEYBOARD = 1
} InterruptRequest;

typedef struct {
    u8 code[TRAMPOLINE_SIZE];
} Trampoline;

typedef struct {
    u32 edi,
        esi,
        ebp,
        esp,
        ebx,
        edx,
        ecx,
        eax;
    u32 gs,
        fs,
        es,
        ds;
    alignas(4) u8 vector;
    u32 error_code;
    u32 eip;
    u32 cs;
    u32 eflags;
} Context;

typedef struct {
    u16 offset_0_15: 16;
    u16 segment_selector: 16;
    u8 reserved: 5;
    u8 fixed1: 3;
    u8 type: 3;
    u8 d: 1;
    u8 fixed2: 1;
    u8 dpl: 2;
    u8 p: 1;
    u16 offset_16_31: 16;
} InterruptDesc;

typedef void (*custom_handler) (Context*);

void universal_handler(Context* ctx);
void init_interrupts(GateType interrupt_type);
void pic8259_init(PIC8259Type, bool auto_eoi);
void pic8259_enable_device(InterruptRequest irq, custom_handler handler);
void pic8259_disable_device(InterruptRequest irq);
void pic8259_send_EOI(InterruptRequest irq);

#endif
