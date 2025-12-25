#include <stddef.h>
#include "paging.h"
#include "alloc.h"
#include "types.h"
#include "asm_utils.h"
#include "experiments.h"
#include "mem.h"
#include "panic.h"
#include "printf.h"

PageDirectoryEntry* pd;

u8* freed = NULL;
u8* cursor = (u8*) POOL_START;

static void* alloc_page() {
    if (freed != NULL) {
        u8* res = freed;
        freed = *(u8**)res;
        return res;
    }
    if (cursor + PAGE <= (u8*) POOL_END) {
        u8* res = cursor;
        cursor += PAGE;
        return res;
    }
    kernel_panic("Out of memory");
    return NULL;
}

static void free_page(void* block) {
    if (!block) { return; }
    *(u8**)block = freed;
    freed = block;
}

static void* calloc_page() {
    u8* block = alloc_page();
    return memzero(block, PAGE);
}

void init_paging() {
    INIT_PAGING(EXP_NUM);
}

void* alloc_user_stack() {
    ALLOC_USER_STACK(EXP_NUM);
}

void cleanup_user_stack() {
    turn_paging_off();
    PageTableEntry* pt = (PageTableEntry*) (pd[1].pt_addr << 12);
    bool print_flag = false;
    for (u32 i = 0; i < 1024; i++) {
        if (pt[i].p) {
            if (EXP_NUM == 11) {
                if (!print_flag) {
                    printf("%x\n", 0x400000 + i * PAGE);
                    print_flag = true;
                }
            }
            free_page((void*) (pt[i].frame_addr << 12));
        }
    }
    pd[1].p = false;
    free_page(pt);
    turn_paging_on(pd);
}

void expand_user_stack(u32 addr) {
    turn_paging_off();
    PageTableEntry* pt = (PageTableEntry*) (pd[1].pt_addr << 12);
    u32 target = (addr - 0x400000) / PAGE;
    for (i32 i = 1023; i >= (i32) target; --i) {
        if (!pt[i].p) {
            pt[i].frame_addr = (u32) calloc_page() >> 12;
            pt[i].p = true;
            pt[i].r_w = true;
            pt[i].u_s = true;
        }
    }
    turn_paging_on(pd);
}
