#ifndef STDLIB_H
#define STDLIB_H
#include <stdarg.h>
#include "types.h"

void vprintf(const char* fmt, va_list args);
void printf(const char* fmt, ...);
void exit(u32 status);
void print_char(char c);

#endif