#ifndef PAGING_H
#define PAGING_H
#include <stdbool.h>
#include "types.h"

#define USER_STACK_POINTER 0x800000
#define MB (2 << 20)
#define RAM (RAM_MB * MB)
#define PAGE 4096
#define POOL_START 0x400000
#define POOL_END RAM

typedef struct {
    bool p : 1;
    bool r_w : 1;
    bool u_s : 1;
    u16 available : 9;
    u32 frame_addr : 20;
} PageTableEntry;

typedef struct {
    bool p : 1;
    bool r_w : 1;
    bool u_s : 1;
    u32 available1 : 4;
    bool ps : 1;
    u32 available2 : 4;
    u32 pt_addr : 20;
} PageDirectoryEntry;

extern PageDirectoryEntry* pd;

void init_paging();
void* alloc_user_stack();
void cleanup_user_stack();
void expand_user_stack(u32 addr);

#endif