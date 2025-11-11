#include "types.h"

void* memmove(void* dst, const void* src, u32 n) {
    if (dst == src) return dst;
    u8* d = dst;
    const u8* s = src;
    if (dst < src) {
        for (u32 i = 0; i < n; ++i) {
            d[i] = s[i];
        }
    } else {
        for (u32 i = n; i > 0; --i) {
            d[i - 1] = s[i - 1];
        } 
    }
    return d;
}    

void* memzero(void* dst, u32 n) {
    u8* d = dst;
    for (u32 i = 0; i < n; ++i) {
        d[i] = 0;
    }
    return d;
}
