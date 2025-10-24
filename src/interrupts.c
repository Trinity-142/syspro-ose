#include "interrupts.h"

#include <stdbool.h>

#include "printf.h"
#include "alloc.h"
#include "panic.h"
#include "types.h"
#include "asm_utils.h"
#include "assert.h"

bool has_error_code(u32 vector) {
    const u8 has_error_code[] = {8, 10, 11, 12, 13, 14, 17, 21};
    for (int i = 0; i < sizeof(has_error_code); ++i) {
        if (vector == has_error_code[i]) {
            return true;
        }
    }
    return false;
}

Trampoline* setup_trampolines() {
    Trampoline* trampolines = malloc_undead(sizeof(Trampoline) * 256, 8);
    for (u16 vector = 0; vector < 256; ++vector) {
        Trampoline* trampoline = &trampolines[vector];
        u8 offset = 0;
        if (!has_error_code(vector)) {
            trampoline->code[offset++] = 0x50;
        }
        trampoline->code[offset++] = 0x6A;   // push
        trampoline->code[offset++] = vector; // immediate = vector
        trampoline->code[offset++] = 0xE9;   // jmp
        const u32 collect_context_offset = (u32) collect_context - (u32) &trampoline->code[offset + 4]; // offset to collect_context
        *(u32*) &trampoline->code[offset] = collect_context_offset;
        offset += 4;
        assert(offset <= TRAMPOLINE_SIZE);
    }
    return trampolines;
}

void setup_idt(const Trampoline* trampolines) {
    assert(sizeof(InterruptDesc) == 8);
    InterruptDesc *idt = malloc_undead(sizeof(InterruptDesc) * 256, 8);
    for (int vector = 0; vector < 256; ++vector) {
        InterruptDesc desc;
        u32 handler_addr = (u32) trampolines[vector].code;
        desc.offset_0_15 = handler_addr & 0xFFFF;
        desc.segment_selector = 8;
        desc.reserved = 0;
        desc.fixed1 = 0;
        desc.type = INTERRUPT_GATE;
        desc.d = 1;
        desc.fixed2 = 0;
        desc.dpl = 0;
        desc.p = 1;
        desc.offset_16_31 = handler_addr >> 16 & 0xFFFF;
        idt[vector] = desc;
    }
    u64 pseudo_desc = (u64) idt << 16 | 256 * sizeof(InterruptDesc) - 1;
    lidt(&pseudo_desc);
}

void init_interrupts() {
    setup_idt(setup_trampolines());
}

void universal_handler(const Context *ctx) {
    assert(sizeof(Context) == 68);
    kernel_panic("Kernel panic: unhandled interrupt #%d at 0x%x:0x%x\n\n"
                 "Registers:\n"
                 "  EAX: 0x%x, ECX: 0x%x, EDX: 0x%x, EBX: 0x%x\n"
                 "  ESP: 0x%x, EBP: 0x%x, ESI: 0x%x, EDI: 0x%x\n"
                 "  DS : 0x%x, ES : 0x%x, FS : 0x%x, GS : 0x%x\n\n"
                 "Error code: 0x%x\n\n"
                 "EFLAGS:\n"
                 "  value: 0x%x",
                 ctx->vector, ctx->cs, ctx->eip,
                 ctx->eax, ctx->ecx, ctx->edx, ctx->ebx,
                 ctx->esp, ctx->ebp, ctx->esi, ctx->edi,
                 ctx->ds, ctx->es, ctx->fs, ctx->gs,
                 ctx->error_code, ctx->eflags);
}
