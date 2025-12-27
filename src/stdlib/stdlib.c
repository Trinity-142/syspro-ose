#include "stdlib.h"
#include "types.h"
#include <stdbool.h>

void print_string(const char* str) {
    for (const char* c = str; *c; ++c) {
        print_char(*c);
    }
}

void print_unsigned(u32 number, u32 radix) {
    u32 i = 33;
    char string[i];
    string[--i] = '\0';

    do {
        u32 digit = number % radix;
        number /= radix;
        string[--i] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
    } while (number);
    print_string(&string[i]);
}

void print_signed(i32 number) {
    if (number < 0) {
        print_char('-');
        number *= -1;
    }
    print_unsigned(number, 10);
}

void vprintf(const char* fmt, va_list args) {
    bool percent = false;

    for (const char* c = fmt; *c; ++c) {
        if (percent) {
            switch (*c) {
                case 'd': {
                    i32 number = va_arg(args, i32);
                    print_signed(number);
                    break;
                }
                case 'x': {
                    u32 number = va_arg(args, u32);
                    printf("0x");
                    print_unsigned(number, 16);
                    break;
                }
                case 'b': {
                    u32 number = va_arg(args, u32);
                    printf("0b");
                    print_unsigned(number, 2);
                    break;
                }
                case 'c': {
                    char c = va_arg(args, i32);
                    print_char(c);
                    break;
                }
                case 's': {
                    const char* string = va_arg(args, const char*);
                    print_string(string);
                    break;
                }
                case '%':
                    print_char('%');
                    break;

                default:
                    break;
            }
            percent = false;

        } else {
            switch (*c) {
                case '%':
                    percent = true;
                    break;
                default:
                    print_char(*c);
                    break;
            }
        }
    }
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
