#ifndef ASM_UTILS_H
#define ASM_UTILS_H

#include "interrupts.h"
#include "pic8259.h"
#include "types.h"

void endless_loop();
void cli();
void lidt(u64* pseudo_desc);
void collect_context();
void set_gprs();
void division_by_zero();
void syscall();
void sti();
void write_u8(Port port, u8 value);
u8 read_u8(Port port);
void cpuid();
void enter_userspace(void (*user_entry)(), void* user_stack);

#endif
