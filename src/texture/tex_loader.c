#include "tex_loader.h"

bool chk_tex_loader_alloc(TexLoader *tex_loader, size_t memory_size)
{
    chk_error_if(!tex_loader, "tex_loader was NULL.") return false;
    void *memory = chk_alloc(memory_size);
    chk_error_if(!memory, "Failed to allocate %zu bytes for the texture loader backend.") return false;

    tex_loader->owns_memory = true;
    bool initted = chk_tex_loader_init(tex_loader, memory, memory_size);
    return initted;
}

bool chk_tex_loader_init(TexLoader *tex_loader, void *memory, size_t memory_size)
{
}

void chk_tex_loader_destroy(TexLoader *tex_loader)
{
}
