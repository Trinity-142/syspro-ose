#ifndef ASM_UTILS_H
#define ASM_UTILS_H
#include "types.h"

void endless_loop();
void cli();
void lidt(u64* pseudo_desc);
void collect_context();
void set_gprs();
void division_by_zero();
void syscall();
void sti();

#endif
