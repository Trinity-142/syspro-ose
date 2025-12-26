#ifndef VGA_H
#define VGA_H

#include "types.h"

#define VGA_BUFFER 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define BYTES_PER_SYMBOL 2
#define VGA_SIZE VGA_WIDTH * VGA_HEIGHT * BYTES_PER_SYMBOL 

#define BACKGROUND_COLOR 0x0
#define FOREGROUND_COLOR 0xA
#define BLINK_FLAG 0
#define ATTRIBUTE ((BLINK_FLAG << 7) | (BACKGROUND_COLOR << 4) | FOREGROUND_COLOR)

typedef struct {
    u32 x;
    u32 y;
} Coords;

typedef struct {
    Coords start;
    Coords size;
    Coords cursor;
} Console;

extern Console consoles[4];

void vga_clear_screen();
void init_console(Console* console, Coords start, Coords size);
void vga_print_char(char c, Coords cursor);
void vga_scroll_down();
void draw_borders(Console* console);

#endif
