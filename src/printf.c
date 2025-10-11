#include <stdarg.h>
#include <stdbool.h>

#include "assert.h"
#include "panic.h"
#include "types.h"
#include "vga.h"

static u32 x = 0;
static u32 y = 0;

void fixscreen() {
    if (x >= VGA_WIDTH) {
        x = 0;
        y++;
    }

    while (y >= VGA_HEIGHT) {
        vga_scroll_down();
        y -= 1;
    }
}

void putchar(char c) {
    vga_print_char(c, x, y);
    x++;
    fixscreen();
}

void print_string(const char* str) {
    for (const char* c = str; *c; ++c) {
        putchar(*c);
    }
} 

void print_unsigned(u32 number, u32 radix) {
    char string[33];
    u32 i = 32; 
    string[i] = '\0';

    do {
        assert(i);
        u32 digit = number % radix;
        number /= radix;
        string[--i] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
    } while (number); 
    
    print_string(&string[i]);
}

void init_printer() {
    vga_clear_screen();
    x = 0;
    y = 0; 
}

void vprintf(const char* fmt, va_list args) {
    bool percent = false;

    for (const char* c = fmt; *c; ++c) {
        if (percent) {
            switch (*c) {
                case 'd': {
                    i32 number = va_arg(args, i32);
                    if (number < 0) {
                        putchar('-');
                        number *= -1;
                    }
                    print_unsigned(number, 10);
                    break;
                }
                case 'x': {
                    u32 number = va_arg(args, u32);
                    print_unsigned(number, 16);
                    break;
                }
                case 'c': {
                    char c = va_arg(args, i32);
                    putchar(c);
                    break; 
                }
                case 's': {
                    const char* string = va_arg(args, const char*);
                    print_string(string);
                    break;
                }
                case '%': 
                    putchar('%');
                    break;
            
                default:
                    kernel_panic("Kernel panic: uknown type specifier: %c\n", *c);
            }
            percent = false;

        } else {
            switch (*c) {
                case '\n': {
                    x = 0;
                    y += 1;
                    fixscreen();
                    break;
                }
                case '\r':
                    x = 0;
                    break;
                case '%':
                    percent = true;
                    break;
                default:
                    putchar(*c);
                    break;
            }    
        }
    } 
    va_end(args);
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
}
