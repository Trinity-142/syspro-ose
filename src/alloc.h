#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>

void* malloc_undead(size_t size, size_t alignment);

void* calloc_undead(size_t size, size_t alignment);

#endif