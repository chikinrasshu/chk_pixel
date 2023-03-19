#pragma once

#include <arena.h>
#include <colors.h>
#include <common.h>
#include <math/vec2.h>

// List of Rendering commands stored in a contiguous arena that's free'd every frame
typedef struct s_CmdList
{
    MemoryArena arena;
    size_t count;
} CmdList;

CmdList *chk_cmd_list_create(size_t size);
void chk_cmd_list_free(CmdList *cmd_list);

bool chk_cmd_list_init(CmdList *cmd_list, size_t size);
void chk_cmd_list_destroy(CmdList *cmd_list);

// Push commands
void push_line(CmdList *cmd_list, V2 p0, V2 p1, RGBA color);
void push_line_grad(CmdList *cmd_list, V2 p0, V2 p1, RGBA c0, RGBA c1);
void push_quad(CmdList *cmd_list, V2 p0, V2 p1, RGBA color);
void push_tex_quad(CmdList *cmd_list, V2 p0, V2 p1, UV uv0, UV uv1);
