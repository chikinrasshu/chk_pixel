#pragma once

#include <common.h>
#include <arena.h>

#include <spng.h>

typedef struct s_TexLoader
{
    void *mem_block;
    size_t mem_size;
    bool owns_memory;

    spng_ctx *ctx;
} TexLoader;

bool chk_tex_loader_alloc(TexLoader *tex_loader, size_t memory_size);
bool chk_tex_loader_init(TexLoader *tex_loader, void *memory, size_t memory_size);
void chk_tex_loader_destroy(TexLoader *tex_loader);
