#pragma once

#include <stddef.h>

void* malloc_undead(size_t size, size_t alignment);

void* calloc_undead(size_t size, size_t alignment);
