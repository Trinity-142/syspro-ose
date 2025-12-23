#include "paging.h"


#include "types.h"
#include "allocator.h"
#include "asm_utils.h"
#include "printf.h"

PageDirectoryEntry* pd;

void init_paging() {
    pd = calloc_block();
    PageTableEntry* pt = calloc_block();
    pd->pt_addr = (u32) pt >> 12;
    pd->p = true;
    pd->r_w = true;
    pd->u_s = true;
    pd->ps = false;
    for (u32 i = 0; i < 1024; i++) {
        pt[i].frame_addr = (i * PAGE) >> 12;
        pt[i].p = true;
        pt[i].r_w = true;
        if (pt[i].frame_addr < 0x7 || (pt[i].frame_addr >= 0x80 && pt[i].frame_addr < 0x400)) {
            pt[i].u_s = false;
        } else {
            pt[i].u_s = true;
        }
    }
    turn_paging_on(pd);
}

void* alloc_user_stack() {
    turn_paging_off();
    PageTableEntry* pt = calloc_block();
    pd[1].pt_addr = (u32) pt >> 12;
    pd[1].p = true;
    pd[1].r_w = true;
    pd[1].u_s = true;
    pd[1].ps = false;

    //pt[1023].frame_addr = (u32) calloc_block() >> 12;
    //pt[1023].p = true;
    //pt[1023].r_w = true;
    //pt[1023].u_s = true;
    turn_paging_on(pd);
    return (void*) USER_STACK_POINTER;
}

void cleanup_user_stack() {
    turn_paging_off();
    PageTableEntry* pt = (PageTableEntry*) (pd[1].pt_addr << 12);
    bool f = false;
    for (u32 i = 0; i < 1024; i++) {
        if (pt[i].p) {
            if (!f) {
                printf("%x\n", 0x400000 + i * PAGE);
                f = true;
            }
            free_block((void*) (pt[i].frame_addr << 12));
        }
    }
    pd[1].p = false;
    free_block(pt);
    turn_paging_on(pd);
}

void expand_user_stack(u32 addr) {
    turn_paging_off();
    PageTableEntry* pt = (PageTableEntry*) (pd[1].pt_addr << 12);
    if (addr < 0x400000) return;
    u32 target = (addr - 0x400000) / PAGE;
    for (i32 i = 1023; i >= target; --i) {
        if (!pt[i].p) {
            pt[i].frame_addr = (u32) calloc_block() >> 12;
            pt[i].p = true;
            pt[i].r_w = true;
            pt[i].u_s = true;
        }
    }
    turn_paging_on(pd);
}
