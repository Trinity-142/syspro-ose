#ifndef PANIC_H
#define PANIC_H

#include <stdarg.h>

void vkernel_panic(const char* fmt, va_list args);

void kernel_panic(const char* fmt, ...);

#endif
