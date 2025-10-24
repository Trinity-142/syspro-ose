#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#define INTERRUPT_GATE 0b110
#define TRAP_GATE 0b111
#include <stdalign.h>

#include "types.h"

struct idt_ptr {
    u16 limit;
    u32 base;
} __attribute__((packed));

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
} context;

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
} interrupt_desc;

void universal_handler(const context* ctx);
void init_interrupts();

#endif