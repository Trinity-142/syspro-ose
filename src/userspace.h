#ifndef USERSPACE_H
#define USERSPACE_H

#define APP_CODE 0x18
#define APP_DATA 0x20
#define USER_PL 3

void enter_userspace(void (*user_entry)(), void* user_stack);

#endif