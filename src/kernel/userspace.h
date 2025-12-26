#ifndef USERSPACE_H
#define USERSPACE_H

#include "types.h"

#define USER_STACK_POINTER 0x800000
#define USER_CODE_POINTER 0x800000
#define USER_ARGV_POINTER 0x810000
#define APP_CODE 0x18
#define APP_DATA 0x20
#define USER_PL 3
#define KERNEL_PL 0

extern u32 param;

void start_usercode();
void enter_userspace(void (*user_entry)(), void* user_stack, int argc, char** argv);

#endif