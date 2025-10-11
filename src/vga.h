#pragma once

#include <stddef.h>

#define VGA_BUFFER 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define BYTES_PER_SYMBOL 2
#define VGA_SIZE VGA_WIDTH * VGA_HEIGHT * BYTES_PER_SYMBOL 

#define BACKGROUND_COLOR 0x0
#define FOREGROUND_COLOR 0xA
#define BLINK_FLAG 0
#define ATTRIBUTE ((BLINK_FLAG << 7) | (BACKGROUND_COLOR << 4) | FOREGROUND_COLOR)

void vga_clear_screen();

void vga_print_char(char c, size_t x, size_t y);

void vga_scroll_down();
