#ifndef ASM_UTILS_H
#define ASM_UTILS_H

#include "interrupts.h"
#include "pic8259.h"
#include "types.h"
#include "paging.h"

void endless_loop();
void cli();
void lidt(u64* pseudo_desc);
void collect_context();
void set_gprs();
void division_by_zero();
void print_char(char c);
void sti();
void write_u8(Port port, u8 value);
u8 read_u8(Port port);
void cpuid();
void restore_user_context(Context* ctx);
u32 get_eflags();
void lgdt();
void ltr();
void cr0();
u32 get_esp();
void turn_paging_on();
void turn_paging_off();
void set_cr3(PageDirectoryEntry* pd);
u32 get_cr2();
void endless_rec();

#endif
