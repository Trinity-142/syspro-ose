#pragma once

#include "panic.h"

#ifdef DEBUG
    #define assert(cond) \
        ((cond) ? (void)0 : kernel_panic("Kernel panic: assertion failed: %s at (%s, line %d\n", #cond, __FILE__, __LINE__))
#else
    #define assert(cond) ((void)0)
#endif
