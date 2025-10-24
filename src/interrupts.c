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

u8* setup_trampolines() {
    u8* trampolines = malloc_undead(8 * 256, 8);
    for (u32 vector = 0; vector < 256; ++vector) {
        u8* trampoline = trampolines + vector * 8;
        u32 offset = 0;
        if (!has_error_code(vector)) {
            trampoline[offset++] = 0x50;
        }
        trampoline[offset++] = 0x6A;   // push
        trampoline[offset++] = vector; // immediate = vector
        trampoline[offset++] = 0xE9;   // jmp
        const u32 collect_context_offset = (u32) collect_context - (u32) (trampoline + offset + 4); // collect_context
        *(u32*) (trampoline + offset) = collect_context_offset;
        offset += 4;
        assert(offset <= 8);
    }
    return trampolines;
}

void setup_idt(const u8* trampolines) {
    assert(sizeof(interrupt_desc) == 8);
    interrupt_desc *idt = malloc_undead(sizeof(interrupt_desc) * 256, 8);
    for (int vector = 0; vector < 256; ++vector) {
        interrupt_desc desc;
        const u32 trampoline = (u32) (trampolines + vector * 8);
        desc.offset_0_15 = trampoline & 0xFFFF;
        desc.segment_selector = 8;
        desc.reserved = 0;
        desc.fixed1 = 0;
        desc.type = INTERRUPT_GATE;
        desc.d = 1;
        desc.fixed2 = 0;
        desc.dpl = 0;
        desc.p = 1;
        desc.offset_16_31 = trampoline >> 16 & 0xFFFF;
        idt[vector] = desc;
    }
    struct idt_ptr ptr = {
        .limit = 256 * sizeof(interrupt_desc) - 1,
        .base = (u32)idt
    };
    u64 pseudo_desc = (u64) idt << 16 | 256 * sizeof(interrupt_desc) - 1;
    lidt(&ptr);
}

void init_interrupts() {
    setup_idt(setup_trampolines());
}

void universal_handler(const context *ctx) {
    assert(sizeof(context) == 68);
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
