#include "ArenaAllocation.h"
#include <stdlib.h>

static size_t align_forward(size_t ptr, size_t align)
{
    size_t mod = ptr % align;

    if(mod != 0)
        ptr += align - mod;

    return ptr;
}

Arena arena_create(size_t size)
{
    Arena a;

    a.buffer = (uint8_t*)malloc(size);
    a.capacity = size;
    a.offset = 0;

    return a;
}

void arena_destroy(Arena *arena)
{
    free(arena->buffer);

    arena->buffer = NULL;
    arena->capacity = 0;
    arena->offset = 0;
}

void arena_reset(Arena *arena)
{
    arena->offset = 0;
}

void *arena_alloc(Arena *arena, size_t size, size_t align)
{
    size_t current = arena->offset;
    size_t aligned = align_forward(current, align);

    if(aligned + size > arena->capacity)
        return NULL;

    void *ptr = arena->buffer + aligned;

    arena->offset = aligned + size;

    return ptr;
}