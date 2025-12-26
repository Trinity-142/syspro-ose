#ifndef PRINTF_H
#define PRINTF_H

#include <stdarg.h>

#include "types.h"

extern u32 x;
extern u32 y;
void set_cursor(u32 X, u32 Y);

void init_printer();

void vprintf(const char* fmt, va_list args);

void printf(const char* fmt, ...);

void fixscreen();

#endif
