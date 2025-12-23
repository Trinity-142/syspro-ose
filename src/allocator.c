#include "allocator.h"

#include <stddef.h>

#include "mem.h"
#include "panic.h"

u8* freed;
void init_allocator(u8* arena_start, u8* arena_end, u32 block_size) {
    freed = arena_start;

    u8* curr = arena_start;
    while (curr + block_size < arena_end) {
        u8* next = curr + block_size;
        if (next + block_size < arena_end) {
            *(u8**)curr = next;
        } else *(u8**)curr = NULL;
        curr += block_size;
    }
}

void* alloc_block() {
    u8* res = freed;
    if (res != NULL) {
        freed = *(u8**)res;
        return res;
    }
    kernel_panic("Out of memory");
    return NULL;
}

void free_block(void* block) {
    if (!block) { return; }
    *(u8**)block = freed;
    freed = block;
}

void* calloc_block() {
    u8* block = alloc_block();
    return memzero(block, PAGE);
}
