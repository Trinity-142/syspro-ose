#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "types.h"

#define GB 2 << 30
#define MB (2 << 20)
#define KB 2 << 10

#define POOL_START 0x400000
#ifndef RAM_MB
#define RAM_MB 5
#endif
#define RAM (RAM_MB * MB)
#define PAGE 4096


typedef struct {
    u8* freed;
} Arena;

void init_allocator(u8* arena_start, u8* arena_end, u32 block_size);
void* alloc_block();
void free_block(void* block);
void* calloc_block();

#endif