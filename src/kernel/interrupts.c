#include <stdbool.h>

#include "interrupts.h"
#include "alloc.h"
#include "panic.h"
#include "types.h"
#include "asm_utils.h"
#include "assert.h"
#include "experiments.h"
#include "pic8259.h"
#include "printf.h"
#include "userspace.h"
#include "vga.h"

static bool has_error_code(u32 vector) {
    switch (vector) {
        case 8:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 17:
        case 21:
            return true;
        default:
            return false;
    }
}

static Trampoline* setup_trampolines() {
    Trampoline* trampolines = malloc_undead(sizeof(Trampoline) * 256, 8);
    for (u16 vector = 0; vector < 256; ++vector) {
        Trampoline* trampoline = &trampolines[vector];
        u8 offset = 0;
        if (!has_error_code(vector)) {
            trampoline->code[offset++] = 0x50;  // push eax (fake error code)
        }
        // push vector
        trampoline->code[offset++] = 0x6A;
        trampoline->code[offset++] = vector;

        trampoline->code[offset++] = 0xE9;   // jmp opcode
        // calculate offset from address after jmp to collect_context
        const u32 collect_context_offset = (u32) collect_context - (u32) &trampoline->code[offset + 4]; // 4 bytes of address to jmp
        *(u32*) &trampoline->code[offset] = collect_context_offset;
        offset += 4;
        assert(offset <= TRAMPOLINE_SIZE);
    }
    return trampolines;
}

InterruptDesc* idt;
static void setup_idt(Trampoline* trampolines, GateType interrupt_type) {
    assert(sizeof(InterruptDesc) == 8);
    idt = malloc_undead(sizeof(InterruptDesc) * 256, 8);
    for (int vector = 0; vector < 256; ++vector) {
        InterruptDesc desc;
        u32 handler_addr = (u32) trampolines[vector].code;
        desc.offset_0_15 = handler_addr & 0xFFFF;
        desc.segment_selector = 8;
        desc.reserved = 0;
        desc.fixed1 = 0;
        desc.type = interrupt_type;
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

void init_interrupts(GateType interrupt_type) {
    setup_idt(setup_trampolines(), interrupt_type);
}

void set_interrupt_dpl(u32 vector, u8 dpl) {
    idt[vector].dpl = dpl;
}

static void timer_handler(Context *ctx) {
}

static void keyboard_handler(Context *ctx) {}

static void print_char_handler(Context *ctx) {
    Console* console = current_process->console;
    char c = (char) ctx->eax;
    if (c == '\n') {
        console->cursor.y++;
        console->cursor.x = console->start.x;
    }
    else if (c == '\r') console->cursor.x = console->start.x;
    else {
        vga_print_char(c, console->cursor);
        console->cursor.x++;
    }
    fixscreen();
}

u32 param;
static void exit_handler(Context *ctx) {
    printf("---------\n");
    printf("process exited with code: %d\n", ctx->eax);
    cleanup_user_stack();
    endless_loop();
}

void universal_handler(Context *ctx) {
    assert(sizeof(Context) == 76);
    u8 pl = ctx->cs & 0b11;
    //if (pl == KERNEL_PL) kernel_panic("Kernel panic: interrupt from kernel!\n");
    switch (ctx->vector) {
        case TIMER_VECTOR:
            timer_handler(ctx);
            return;
        case KEYBOARD_VECTOR:
            keyboard_handler(ctx);
            return;
        case PRINT_CHAR_VECTOR:
            print_char_handler(ctx);
            return;
        case EXIT_VECTOR:
            exit_handler(ctx);
            return;
        case PAGE_FAULT_VECTOR:
            bool us = ctx->error_code & (1 << 2);
            assert((pl == 3 && us == 1) || (pl == 0 && us == 0));
            u32 cr2 = get_cr2();
            if (cr2 < 0x200000) printf("NPE\n");
            else if (cr2 < 0x400000) printf("SOE\n");
            else if (cr2 < USER_STACK_POINTER) {
                expand_user_stack(cr2);
                return;
            } else printf("UB: ");
            exit_handler(ctx);
        default:
            print_context(ctx);
    }
}

void print_context(Context* ctx) {
    kernel_panic("Kernel panic: unhandled interrupt #%d at %x:%x\n"
                    "CR2: %x\n\n"
                    "Registers:\n"
                    "  EAX: %x, ECX: %x, EDX: %x, EBX: %x\n"
                    "  ESP: %x, EBP: %x, ESI: %x, EDI: %x\n"
                    "  DS : %x, ES : %x, FS : %x, GS : %x\n\n"
                    "Error code:\n"
                    "  %s, value: %x\n\n"
                    "EFLAGS:\n"
                    "  value: %x",
                    ctx->vector, ctx->cs, ctx->eip,
                    get_cr2(),
                    ctx->eax, ctx->ecx, ctx->edx, ctx->ebx,
                    ctx->esp, ctx->ebp, ctx->esi, ctx->edi,
                    ctx->ds, ctx->es, ctx->fs, ctx->gs,
                    has_error_code(ctx->vector) ? "common" : "fake",
                    ctx->error_code, ctx->eflags);
}
