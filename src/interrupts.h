#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include <stdalign.h>
#include <stdbool.h>

#include "types.h"

#define INTERRUPT_GATE 0b110
#define TRAP_GATE 0b111
#define TRAMPOLINE_SIZE 8

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

void universal_handler(const Context* ctx);
void init_interrupts();

#endif
