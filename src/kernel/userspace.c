#include "userspace.h"
#include "asm_utils.h"
#include "experiments.h"
#include "interrupts.h"

void enter_userspace(void (*user_entry)(), void* user_stack, int argc, char** argv) {
    Context ctx;
    ctx.eax = argc;
    ctx.ecx = (u32) argv;

    ctx.ss_user = APP_DATA | USER_PL;
    ctx.esp_user = (u32) user_stack;
    ctx.eflags = get_eflags() & 0xFFFFCFFF | 0x200; // IOPL = 0; IF = 1
    ctx.cs = APP_CODE | USER_PL;
    ctx.eip = (u32) user_entry;

    ctx.ds = APP_DATA | USER_PL;
    ctx.es = APP_DATA | USER_PL;
    ctx.fs = APP_DATA | USER_PL;
    ctx.gs = APP_DATA | USER_PL;
    restore_user_context(&ctx);
}

static void user_main() {
    USERSPACE_PROCESS(EXP_NUM);
}

void start_usercode() {
    set_cr3(pd);
    turn_paging_on();
    void* entry = alloc_user_code(0x20000);
    void* stack = alloc_user_stack();
    char** argv = alloc_argc_argv(3, "foo", "bar", "baz");
    enter_userspace(entry, stack, 3, argv);
}
