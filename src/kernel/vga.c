#include "mem.h"
#include "types.h"
#include "vga.h"
#include "printf.h"
#include "userspace.h"

Console consoles[4];

void vga_clear_screen() {
    void* vga_buffer = (void*) VGA_BUFFER;
    memzero(vga_buffer, VGA_SIZE);
}

void vga_print_char(char c, Coords cursor) {
    u8* vga_buffer = (u8*) VGA_BUFFER;
    u32 i = BYTES_PER_SYMBOL * (cursor.y * VGA_WIDTH + cursor.x);
    vga_buffer[i] = c;
    vga_buffer[i + 1] = ATTRIBUTE; 
}

void vga_scroll_down() {
    Console current_console = *current_process->console;
    u8* start = (u8*) VGA_BUFFER + current_console.start.y * VGA_WIDTH * BYTES_PER_SYMBOL + current_console.start.x * BYTES_PER_SYMBOL;
    u32 row = (current_console.size.x + 1) * BYTES_PER_SYMBOL;
    u32 i;
    for (i = 0; i < current_console.size.y; i++) {
        memmove(start + VGA_WIDTH * i * BYTES_PER_SYMBOL, start + VGA_WIDTH * (i + 1) * BYTES_PER_SYMBOL, row);
    }
    memzero(start + VGA_WIDTH * i * BYTES_PER_SYMBOL, row);
}

void fixscreen() {
    Console* console = current_process->console;
    if (console->cursor.x > console->start.x + console->size.x) {
        console->cursor.x = console->start.x;
        console->cursor.y++;
    }

    while (console->cursor.y > console->start.y + console->size.y) {
        vga_scroll_down();
        console->cursor.y--;
    }
}

void init_console(Console* console, Coords start, Coords size) {
    console->start = start;
    console->size = size;
    console->cursor = start;
    draw_borders(console);
}

void draw_borders(Console* console) {
    vga_print_char(218, console->cursor);
    for (u32 i = 0; i < console->size.x - 1; i++) {
        console->cursor.x += 1;
        vga_print_char(196, console->cursor);
        console->cursor.y += console->size.y;
        vga_print_char(196, console->cursor);
        console->cursor.y = console->start.y;
    }
    console->cursor.x++;
    vga_print_char(191, console->cursor);
    console->cursor.x = console->start.x;
    for (u32 i = 0; i < console->size.y - 1; i++) {
        console->cursor.y += 1;
        vga_print_char(179, console->cursor);
        console->cursor.x += console->size.x;
        vga_print_char(179, console->cursor);
        console->cursor.x = console->start.x;
    }

    vga_print_char(179, console->cursor);
    console->cursor.y++;
    vga_print_char(192, console->cursor);
    console->cursor.x += console->size.x;
    vga_print_char(217, console->cursor);

    console->start = (Coords){console->start.x + 1, console->start.y + 1};
    console->size = (Coords){console->size.x - 2, console->size.y - 2};
    console->cursor = console->start;
}
