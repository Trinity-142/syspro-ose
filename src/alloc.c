#include <stddef.h>
#include <stdint.h>

#include "panic.h"
#include "types.h"
#include "mem.h"

#define ARENA_START 0x100000
#define ARENA_END   0x400000
#define DEFAULT_ALIGNMENT 1

static u8 *current = (u8*) ARENA_START;

void *malloc_undead(size_t size, size_t alignment) {
    if (size == 0) return NULL;
    if (alignment == 0) alignment = DEFAULT_ALIGNMENT;

    if ((size_t) current % alignment != 0) {
        current += alignment - (size_t) current % alignment;
    }

    if ((size_t) current + size >= ARENA_END) {
        kernel_panic("Kernel panic: not enough memory to allocate %d bytes\n", size);
    }

    u8 *res = current;
    current += size;
    return res;
}

void *calloc_undead(size_t size, size_t alignment) {
    u8 *res = malloc_undead(size, alignment);
    if (res == NULL) return NULL;
    memzero(res, size);
    return res;
}
