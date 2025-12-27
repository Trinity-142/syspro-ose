#ifndef PRINTF_H
#define PRINTF_H

#include <stdarg.h>
#include "vga.h"
#include "types.h"

void set_cursor(Coords cursor);
void vprintf(const char* fmt, va_list args);
void printf(const char* fmt, ...);
void fixscreen();

#endif
