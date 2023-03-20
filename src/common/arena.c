#include "arena.h"

bool chk_memory_arena_alloc(MemoryArena *arena, size_t size)
{
    chk_error_if(size == 0, "size was zero.") return false;

    void *ptr = chk_alloc(size);
    chk_error_if(!ptr, "Failed to create the memory arena.") return false;

    arena->owns_memory = true;
    if (!chk_memory_arena_init(arena, ptr, size))
    {
        chk_free(ptr);
        return false;
    }
    return true;
}

bool chk_memory_arena_init(MemoryArena *arena, void *memory, size_t size)
{
    chk_error_if(!arena, "arena was NULL.") return false;
    chk_error_if(!memory, "memory was NULL.") return false;
    chk_error_if(!size, "size was zero.") return false;

    if (arena->owns_memory && arena->memory != memory)
        chk_memory_arena_destroy(arena);

    arena->memory = memory;
    arena->size = size;
    arena->used = 0;
    return true;
}

void chk_memory_arena_destroy(MemoryArena *arena)
{
    chk_error_if(!arena, "arena was NULL.") return;

    if (arena->owns_memory)
        chk_free(arena->memory);
    *arena = (MemoryArena){0};
}

void *chk_memory_arena_push(MemoryArena *arena, size_t amount)
{
    chk_error_if(!arena, "arena was NULL.") return NULL;
    chk_error_if(!amount, "amount was zero.") return NULL;
    chk_error_if(arena->size - arena->used < amount, "There is not enough space for the allocation.") return NULL;

    void *ptr = (uint8_t *)arena->memory + arena->used;
    arena->used += amount;
    return ptr;
}

void chk_memory_arena_reset(MemoryArena *arena, bool should_clear)
{
    chk_error_if(!arena, "arena was NULL.") return;

    if (should_clear)
        chk_zero_memory(arena->memory, arena->used);
    arena->used = 0;
}
