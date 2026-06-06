#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../Shared/print.h"


#define ARENA_SIZE (64 * 1024 * 1024)

typedef struct Arena {
    uint8_t* buffer;
    size_t capacity;
    size_t offset;
} Arena;

Arena arena_create(void)
{
    Arena arena;

    arena.buffer = malloc(ARENA_SIZE);
    arena.capacity = ARENA_SIZE;
    arena.offset = 0;

    return arena;
}

void arena_destroy(Arena* arena)
{
    free(arena->buffer);

    arena->buffer = NULL;
    arena->capacity = 0;
    arena->offset = 0;
}

static size_t align_forward(size_t ptr, size_t align)
{
    size_t p = ptr;
    size_t modulo = p % align;

    if (modulo != 0)
        p += align - modulo;

    return p;
}

void* arena_alloc(Arena* arena, size_t size, size_t align)
{
    size_t current = arena->offset;
    size_t aligned = align_forward(current, align);

    if (aligned + size > arena->capacity)
        return NULL;

    void* ptr = arena->buffer + aligned;

    arena->offset = aligned + size;

    return ptr;
}

int main(void) {
    Arena arena = arena_create();

    int* x = arena_alloc(&arena, sizeof(int), _Alignof(int));
    char* text = arena_alloc(&arena, 128, _Alignof(char));
    int* y = arena_alloc(&arena, sizeof(int), _Alignof(int));

    if (!x || !text || !y) {
        return 1;
    }

    *x = 42;
    *y = 1337;

    strcpy(text, "Arena allocator funcionando");

    clem_print(PRINT_STRING, "\n=== TEST 1 ===\n");
    clem_print(PRINT_INT, x);
    clem_print(PRINT_STRING, "\n");
    clem_print(PRINT_INT, y);
    clem_print(PRINT_STRING, "\n");
    clem_print(PRINT_STRING, text);

    /* Verificar que una escritura posterior no corrompe memoria previa */
    *x = 99;

     clem_print(PRINT_STRING, "\n");
    clem_print(PRINT_STRING, "=== TEST 2 ===");
     clem_print(PRINT_STRING, "\n");
    clem_print(PRINT_INT, x);
     clem_print(PRINT_STRING, "\n");
    clem_print(PRINT_INT, y);
     clem_print(PRINT_STRING, "\n");
    clem_print(PRINT_STRING, text);

    arena_destroy(&arena);
    return 0;
}