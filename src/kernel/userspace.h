#ifndef USERSPACE_H
#define USERSPACE_H

#include "alloc.h"
#include "interrupts.h"
#include "paging.h"
#include "types.h"
#include "vga.h"

#define USER_STACK_POINTER 0x800000
#define USER_CODE_POINTER 0x800000
#define USER_ARGV_POINTER 0x810000
#define APP_CODE 0x18
#define APP_DATA 0x20
#define USER_PL 3
#define KERNEL_PL 0

typedef struct {
    Context ctx;
    Console* console;
    PageDirectoryEntry* pd;
} Process;

extern Process processes[4];
extern Process* current_process;

void start_usercode();
void init_context(Context* ctx, void (*user_entry)(), void* user_stack, int argc, char** argv);
void init_curr_process(u32 code_addr, Console* console, int argc, ...);

#endif