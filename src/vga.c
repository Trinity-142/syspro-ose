#include <stddef.h>
#include "mem.h"
#include "types.h"
#include "vga.h"
#include "printf.h"

void vga_clear_screen() {
    void* vga_buffer = (void*) VGA_BUFFER;
    memzero(vga_buffer, VGA_SIZE);
    set_cursor(0, 0);
}

void vga_print_char(char c, size_t x, size_t y) {
    u8* vga_buffer = (u8*) VGA_BUFFER;
    size_t i = BYTES_PER_SYMBOL * (y * VGA_WIDTH + x);
    vga_buffer[i] = c;
    vga_buffer[i + 1] = ATTRIBUTE; 
}

void vga_scroll_down() {
    u8* vga_buffer = (u8*) VGA_BUFFER;
    u32 first_row = VGA_WIDTH * BYTES_PER_SYMBOL;
    memmove(vga_buffer, vga_buffer + first_row, VGA_SIZE - first_row);
    memzero(vga_buffer + VGA_SIZE - first_row, first_row);
}
