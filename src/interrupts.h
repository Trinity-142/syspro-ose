#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include <stdalign.h>

#include "types.h"

#define TRAMPOLINE_SIZE 8
#define WRITE_VECTOR 0x30
#define EXIT_VECTOR 0x31
#define PAGE_FAULT_VECTOR 14

extern u32 global;
extern u32 N;

typedef enum {
    INTERRUPT = 0b110,
    TRAP = 0b111
} GateType;

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
    u32 esp_privileged;
    u32 ss_privileged;
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

void universal_handler(Context* ctx);
void init_interrupts(GateType interrupt_type);
void print_context(Context* ctx);

#endif
