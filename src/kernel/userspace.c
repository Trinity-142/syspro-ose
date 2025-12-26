#include "userspace.h"

#include <stdarg.h>

#include "asm_utils.h"
#include "experiments.h"
#include "interrupts.h"

Process processes[4];
Process* current_process = processes;

void init_context(Context* ctx, void (*user_entry)(), void* user_stack, int argc, char** argv) {
    ctx->eax = argc;
    ctx->ecx = (u32) argv;
    ctx->ss_user = APP_DATA | USER_PL;
    ctx->esp_user = (u32) user_stack;
    ctx->eflags = get_eflags() & 0xFFFFCFFF | 0x200; // IOPL = 0; IF = 1
    ctx->cs = APP_CODE | USER_PL;
    ctx->eip = (u32) user_entry;

    ctx->ds = APP_DATA | USER_PL;
    ctx->es = APP_DATA | USER_PL;
    ctx->fs = APP_DATA | USER_PL;
    ctx->gs = APP_DATA | USER_PL;
}

void start_usercode() {
    set_cr3(current_process->pd);
    turn_paging_on();
    restore_user_context(&current_process->ctx);
}

void init_curr_process(u32 code_addr, Console* console, int argc, ...) {
    current_process->pd = init_pd();
    void* entry = alloc_user_code(code_addr);
    void* stack = alloc_user_stack();

    va_list argv_list;
    va_start(argv_list, argc);
    char** argv = alloc_argc_argv(argc, argv_list);
    va_end(argv_list);

    init_context(&current_process->ctx, entry, stack, argc, argv);

    current_process->console = console;
}
