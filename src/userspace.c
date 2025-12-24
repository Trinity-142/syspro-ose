#include "userspace.h"
#include "asm_utils.h"
#include "interrupts.h"

void enter_userspace(void (*user_entry)(), void* user_stack) {
    Context ctx;

    ctx.ss_privileged = APP_DATA | USER_PL;
    ctx.esp_privileged = (u32) user_stack;
    ctx.eflags = get_eflags() & 0xFFFFCFFF | 0x200; // IOPL = 0; IF = 1
    ctx.cs = APP_CODE | USER_PL;
    ctx.eip = (u32) user_entry;

    ctx.ds = APP_DATA | USER_PL;
    ctx.es = APP_DATA | USER_PL;
    ctx.fs = APP_DATA | USER_PL;
    ctx.gs = APP_DATA | USER_PL;
    restore_user_context(&ctx);
}
