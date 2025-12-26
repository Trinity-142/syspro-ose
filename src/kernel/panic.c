#include <stdarg.h>
#include "printf.h"
#include "asm_utils.h"
#include "userspace.h"
#include "vga.h"

void vkernel_panic(const char* fmt, va_list args) {
    cli(); 
    vprintf(fmt, args);
    endless_loop();
}

void kernel_panic(const char* fmt, ...) {
    vga_clear_screen();
    Console fullscreen = (Console) {
        .start = (Coords) {0, 0},
        .size = (Coords) {VGA_WIDTH, VGA_HEIGHT},
        .cursor = (Coords) {0, 0}
    };
    init_console(&fullscreen, (Coords) {0,0}, (Coords) {VGA_WIDTH, VGA_HEIGHT});
    *current_process->console = fullscreen;
    va_list args;
    va_start(args, fmt);
    vkernel_panic(fmt, args);
    va_end(args);
}
