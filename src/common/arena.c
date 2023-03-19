#include "arena.h"

MemoryArena *chk_memory_arena_create(size_t size)
{
    if (!size)
    {
        fprintf(stderr, "chk_memory_arena_create: Size was 0.\n");
        return NULL;
    }

    MemoryArena *arena = chk_alloc(sizeof(*arena));
    if (!arena)
    {
        fprintf(stderr, "chk_memory_arena_create: Failed to allocate the memory arena object.\n");
    }

    if (!chk_memory_arena_init(arena, size))
    {
        chk_free(arena);
        return NULL;
    }

    return arena;
}

void chk_memory_arena_free(MemoryArena *arena)
{
    chk_memory_arena_destroy(arena);
    chk_free(arena);
}

bool chk_memory_arena_init(MemoryArena *arena, size_t size)
{
    if (!arena)
    {
        fprintf(stderr, "chk_memory_arena_init: arena was NULL\n");
        return false;
    }

    if (arena->memory)
        chk_memory_arena_destroy(arena);

    arena->memory = chk_alloc(size);
    if (!arena->memory)
    {
        fprintf(stderr, "chk_memory_arena_init: Failed to allocate %zu bytes for the memory arena.\n", size);
        return false;
    }

    arena->size = size;
    arena->used = 0;
    return true;
}

void chk_memory_arena_destroy(MemoryArena *arena)
{
    if (arena->memory)
        chk_free(arena->memory);

    arena->memory = NULL;
    arena->used = arena->size = 0;
}

void *chk_memory_arena_push(MemoryArena *arena, size_t amount)
{
    if (arena->used + amount > arena->size)
    {
        fprintf(stderr, "chk_memory_arena_push: Not enough memory to push %zu bytes. There are only %zu left.\n", amount, arena->size - arena->used);
        return NULL;
    }
    uint8_t *ptr = (uint8_t *)arena->memory + arena->used;
    arena->used += amount;
    return ptr;
}
