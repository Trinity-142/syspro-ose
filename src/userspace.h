#ifndef USERSPACE_H
#define USERSPACE_H

#include "types.h"

#define APP_CODE 0x18
#define APP_DATA 0x20
#define USER_PL 3

extern u32 param;

void start_usercode();
void enter_userspace(void (*user_entry)(), void* user_stack);

#endif