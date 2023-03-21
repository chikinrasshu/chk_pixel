#pragma once

#include "common.h"

typedef struct s_MemoryArena
{
    size_t size, used;
    void *memory;
    void *last_alloc;
    bool owns_memory;
} MemoryArena;

bool chk_memory_arena_alloc(MemoryArena *arena, size_t size);
bool chk_memory_arena_zalloc(MemoryArena *arena, size_t size);
bool chk_memory_arena_init(MemoryArena *arena, void *memory, size_t size);
void chk_memory_arena_destroy(MemoryArena *arena);

void *chk_memory_arena_push(MemoryArena *arena, size_t amount);
#define chk_memory_arena_push_struct(Arena, Type) chk_memory_arena_push(Arena, sizeof(Type));
#define chk_memory_arena_push_array(Arena, Type, Count) chk_memory_arena_push(Arena, sizeof(Type) * (Count));

void *chk_memory_arena_zpush(MemoryArena *arena, size_t amount);
#define chk_memory_arena_zpush_struct(Arena, Type) chk_memory_arena_zpush(Arena, sizeof(Type));
#define chk_memory_arena_zpush_array(Arena, Type, Count) chk_memory_arena_zpush(Arena, sizeof(Type) * (Count));

void chk_memory_arena_reset(MemoryArena *arena, bool should_clear);
