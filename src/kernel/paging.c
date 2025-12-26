#include <stddef.h>
#include "paging.h"
#include "alloc.h"
#include "types.h"
#include "asm_utils.h"
#include "experiments.h"
#include "mem.h"
#include "panic.h"
#include "printf.h"
#include "userspace.h"

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
    pd = calloc_page();
    pd->pt_addr = 0;
    pd->p = true;
    pd->r_w = true;
    pd->u_s = true;
    pd->ps = true;
    set_cr3(pd);
    turn_paging_on();
}

void* alloc_user_code(u32 addr) {
    turn_paging_off();
    PageTableEntry* pt = calloc_page();
    pd[2].pt_addr = (u32) pt >> 12;
    pd[2].p = true;
    pd[2].r_w = true;
    pd[2].u_s = true;
    pd[2].ps = false;
    for (u32 i = 0; i < 16; i++) {
        pt[i].frame_addr = (addr + i * PAGE) >> 12;
        pt[i].p = true;
        pt[i].r_w = true;
        pt[i].u_s = true;
    }
    turn_paging_on();
    return (void*) USER_CODE_POINTER;
}

void* alloc_user_stack() {
    turn_paging_off();
    PageTableEntry* pt = calloc_page();
    pd[1].pt_addr = (u32) pt >> 12;
    pd[1].p = true;
    pd[1].r_w = true;
    pd[1].u_s = true;
    pd[1].ps = false;
    turn_paging_on();
    return (void*) USER_STACK_POINTER;
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
    //free_page((void*) (pd[2].pt_addr << 12));
    turn_paging_on();
}

void expand_user_stack(u32 addr) {
    turn_paging_off();
    PageTableEntry* pt = (PageTableEntry*) (pd[1].pt_addr << 12);
    i32 to = ((i32) addr - POOL_START) / PAGE;
    i32 from = (USER_STACK_POINTER - POOL_START) / PAGE - 1;
    for (i32 i = from; i >= to; --i) {
        if (!pt[i].p) {
            pt[i].frame_addr = (u32) calloc_page() >> 12;
            pt[i].p = true;
            pt[i].r_w = true;
            pt[i].u_s = true;
        }
    }
    turn_paging_on();
}
