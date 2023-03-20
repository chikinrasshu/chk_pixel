#include "cmd_list.h"

const char *CmdKindNames[] = {
    "Unknown",
    "Clear",
    "Line",
    "Rect",

    "__Count__",
};

static_assert(chk_array_count(CmdKindNames) != CmdKind_Count,
              "The render command's kinds and their names arrays do not match. Please add or remove them.");

const char *chk_cmd_kind_get_name(CmdKind kind)
{
    return CmdKindNames[kind];
}

bool chk_cmd_list_init(CmdList *cmd_list, void *memory, size_t memory_size)
{
    if (!chk_memory_arena_init(&cmd_list->arena, memory, memory_size))
        return false;

    cmd_list->count = 0;
    return true;
}

void chk_cmd_list_destroy(CmdList *cmd_list)
{
    chk_memory_arena_destroy(&cmd_list->arena);
    cmd_list->count = 0;
}

void chk_cmd_list_reset(CmdList *cmd_list)
{
    chk_memory_arena_reset(&cmd_list->arena, false);
    cmd_list->count = 0;
}

// Push commands
#define CHK_PUSH_CMD(List, TypeName, Color, Bmp)                                       \
    RenderCmd *prev = (RenderCmd *)((uint8_t *)List->arena.memory + List->arena.used); \
                                                                                       \
    TypeName##Cmd *cmd = chk_memory_arena_push_struct(&List->arena, TypeName##Cmd);    \
    chk_error_if(!cmd, "Failed to allocate the " #TypeName " command.") return false;  \
    cmd->base.kind = CmdKind_##TypeName;                                               \
    cmd->base.color = Color;                                                           \
    cmd->base.bmp = Bmp;                                                               \
    cmd->base.next = NULL;                                                             \
    if (prev && prev != &cmd->base)                                                    \
        prev->next = &cmd->base;                                                       \
    ++cmd_list->count

bool chk_push_clear(CmdList *cmd_list, RGBA color)
{
    CHK_PUSH_CMD(cmd_list, Clear, color, NULL);
    return true;
}

bool chk_push_line(CmdList *cmd_list, V2 p0, V2 p1, RGBA color)
{
    return chk_push_line_grad(cmd_list, p0, p1, color, color);
}

bool chk_push_line_grad(CmdList *cmd_list, V2 p0, V2 p1, RGBA c0, RGBA c1)
{
    CHK_PUSH_CMD(cmd_list, Line, c0, NULL);
    cmd->p0 = p0;
    cmd->p1 = p1;
    cmd->color2 = c1;
    return true;
}

bool chk_push_rect(CmdList *cmd_list, V2 p0, V2 p1, RGBA color)
{
    return chk_push_rect_tex(cmd_list, p0, p1, color, NULL);
}

bool chk_push_rect_tex(CmdList *cmd_list, V2 p0, V2 p1, RGBA color, Bitmap *bmp)
{
    return chk_push_rect_tex_uv(cmd_list, p0, p1, color, bmp, v2(0.0f, 0.0f), v2(1.0f, 1.0f));
}

bool chk_push_rect_tex_uv(CmdList *cmd_list, V2 p0, V2 p1, RGBA color, Bitmap *bmp, UV uv0, UV uv1)
{
    CHK_PUSH_CMD(cmd_list, Rect, color, bmp);
    cmd->p0 = p0;
    cmd->p1 = p1;
    cmd->uv0 = uv0;
    cmd->uv1 = uv1;
    return true;
}

#undef CHK_PUSH_CMD