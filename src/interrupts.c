#include "interrupts.h"

#include <stdbool.h>

#include "printf.h"
#include "alloc.h"
#include "panic.h"
#include "types.h"
#include "asm_utils.h"
#include "assert.h"
#include "mem.h"

static custom_handler custom_handlers[IRQs];

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

void setup_idt(Trampoline* trampolines, GateType interrupt_type) {
    assert(sizeof(InterruptDesc) == 8);
    InterruptDesc *idt = malloc_undead(sizeof(InterruptDesc) * 256, 8);
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
    memzero(custom_handlers, sizeof(custom_handlers));
    setup_idt(setup_trampolines(), interrupt_type);
}

void universal_handler(Context *ctx) {
    assert(sizeof(Context) == 68);
    if (ctx->vector >= IRQ0_VECTOR && ctx->vector < IRQ0_VECTOR + IRQs) {
        custom_handler handler = custom_handlers[ctx->vector - IRQ0_VECTOR];
        //printf("%x", handler);
        if (handler && handler != universal_handler) {
            handler(ctx);
            return;
        }
    }
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

void synchronize() {
    //for (int i = 0; i < 5; ++i) cpuid();
    write_0x80();
    write_0x80();
}



void pic8259_init(PIC8259Type type, bool auto_eoi) {
    u8 data_port, command_port;
    if (type == MASTER) {
        command_port = MASTER_COMMAND;
        data_port = MASTER_DATA;
    } else {
        command_port = SLAVE_COMMAND;
        data_port = SLAVE_DATA;
    }

    write_pic8259(data_port, 0b11111111);
    synchronize();
    write_pic8259(command_port, 0b00010001);
    synchronize();
    write_pic8259(data_port, type == MASTER ? IRQ0_VECTOR : IRQ8_VECTOR);
    synchronize();
    write_pic8259(data_port, type == MASTER ? 1 << SLAVE_IRQ : SLAVE_IRQ);
    synchronize();
    write_pic8259(data_port, auto_eoi ? 0b0011 : 0b0001);
    synchronize();
    write_pic8259(data_port, 0b11111111);
}

void pic8259_enable_device(InterruptRequest irq, custom_handler handler) {
    printf("enabling device..\n");
    if (irq < 8) {
        u8 current_mask = read_pic8259(MASTER_DATA);
        printf("current mask: %b\n", current_mask);
        u8 new_mask = ~(1 << irq) & current_mask;
        printf("new mask: %b\n", new_mask);
        write_pic8259(MASTER_DATA, new_mask);
        synchronize();
    } else {
        u8 current_mask = read_pic8259(MASTER_DATA);
        u8 new_mask = ~(1 << (irq-8)) & current_mask;
        write_pic8259(MASTER_DATA, new_mask);
        synchronize();
    }
    printf("irq: %d, handler: %d\n", irq, handler);
    custom_handlers[irq] = handler;
}

void pic8259_send_EOI(InterruptRequest irq) {
    write_pic8259(MASTER_COMMAND, 0x20);
    if (irq >= 8) {
       write_pic8259(SLAVE_COMMAND, 0x20);
    }
}