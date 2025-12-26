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
    //set_cr3(pd);
    //turn_paging_on();
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

u32 strlen(char* str) {
    if (!str) { return 0; }
    u32 i = 0;
    for (const char* c = str; *c; c++) i++;
    return i;
}

char** valloc_argc_argv(int argc, va_list argv) {
    turn_paging_off();
    PageTableEntry* pt = (PageTableEntry*) (pd[2].pt_addr << 12);
    char** argv_page = calloc_page();
    pt[16].frame_addr = (u32) argv_page >> 12;
    pt[16].p = true;
    pt[16].r_w = true;
    pt[16].u_s = true;
    for (u32 i = 0; i < argc; i++) {
        argv_page[i] = (char*) (0x811000 + i * PAGE);
    }

    for (u32 i = 0; i < argc; i++) {
        char* arg_page = calloc_page();
        pt[17 + i].frame_addr = (u32) arg_page >> 12;
        pt[17 + i].p = true;
        pt[17 + i].r_w = true;
        pt[17 + i].u_s = true;
        char* arg = va_arg(argv, char*);
        memmove(arg_page, arg, strlen(arg) + 1);
    }
    turn_paging_on();
    return (char**) USER_ARGV_POINTER;
}

char** alloc_argc_argv(int argc, ...) {
    va_list argv;
    va_start(argv, argc);
    char** res = valloc_argc_argv(argc, argv);
    va_end(argv);
    return res;
}
