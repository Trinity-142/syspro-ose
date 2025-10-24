#pragma once
#include "types.h"

void endless_loop();
void cli();
void lidt(u64* pseudo_desc);
void collect_context();
void set_gprs();
void divide_by_zero();
void syscall();
void sti();