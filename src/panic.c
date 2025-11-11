#include <stdarg.h>
#include "printf.h"
#include "asm_utils.h"
#include "vga.h"

void vkernel_panic(const char* fmt, va_list args) {
    cli(); 
    vprintf(fmt, args);
    va_end(args);
    endless_loop();
}

void kernel_panic(const char* fmt, ...) {
    vga_clear_screen();
    va_list args;
    va_start(args, fmt);
    vkernel_panic(fmt, args);
}
