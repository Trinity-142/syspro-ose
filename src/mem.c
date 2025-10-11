#include <stddef.h>
#include "types.h"

void* memmove(void* dst, const void* src, size_t n) {
    u8* d = dst;
    const u8* s = src;
    if ((size_t) dst < (size_t) src) {
        for (size_t i = 0; i < n; ++i) {
            d[i] = s[i];
        }
    }
    else {
        for (size_t i = 0; i < n; ++i) {
            d[n - i - 1] = s[i];
        } 
    }
    return d;
}    

void* memzero(void* dst, size_t n) {
    u8* d = dst;
    for (size_t i = 0; i < n; ++i) {
        d[i] = 0;
    }
    return d;
}
