#pragma once

#include <stdarg.h>

void init_printer();

void vprintf(const char* fmt, va_list args);

void printf(const char* fmt, ...);
