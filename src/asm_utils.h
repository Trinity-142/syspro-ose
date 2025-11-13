#ifndef ASM_UTILS_H
#define ASM_UTILS_H

#include "interrupts.h"
#include "types.h"

void endless_loop();
void cli();
void lidt(u64* pseudo_desc);
void collect_context();
void set_gprs();
void division_by_zero();
void syscall();
void sti();
void write_pic8259(PIC8259Port port, u8 value);
u8 read_pic8259(PIC8259Port port);
void cpuid();
void write_0x80();

#endif