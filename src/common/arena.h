#pragma once

#include "common.h"

typedef struct s_MemoryArena
{
    size_t size, used;
    void *memory;
} MemoryArena;

MemoryArena *chk_memory_arena_create(size_t size);
void chk_memory_arena_free(MemoryArena *arena);

bool chk_memory_arena_init(MemoryArena *arena, size_t size);
void chk_memory_arena_destroy(MemoryArena *arena);

void *chk_memory_arena_push(MemoryArena *arena, size_t amount);
#define chk_memory_arena_push_struct(Arena, Type) chk_memory_arena_push((arena), sizeof(Type))
#define chk_memory_arena_push_struct_array(Arena, Type, N) chk_memory_arena_push((arena), sizeof(Type) * (N))
