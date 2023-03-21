#pragma once

#include <arena.h>
#include <bitmap.h>
#include <colors.h>
#include <common.h>
#include <math/vec2.h>

typedef enum e_CmdKind
{
    CmdKind_Unknown = 0,
    CmdKind_Clear,
    CmdKind_Line,
    CmdKind_Rect,
    // CmdKind_Generic,

    CmdKind_Count
} CmdKind;

const char *chk_cmd_kind_get_name(CmdKind kind);

typedef struct s_RenderCmd
{
    struct s_RenderCmd *next;
    CmdKind kind;

    RGBA color;
    Bitmap *bmp;
} RenderCmd;

// List of Rendering commands stored in a contiguous arena that's free'd every frame
typedef struct s_CmdList
{
    MemoryArena arena;
    size_t count;
} CmdList;

bool chk_cmd_list_init(CmdList *cmd_list, void *memory, size_t memory_size);
void chk_cmd_list_destroy(CmdList *cmd_list);
void chk_cmd_list_reset(CmdList *cmd_list);

// Render commands
typedef struct s_ClearCmd
{
    RenderCmd base;
} ClearCmd;

typedef struct s_LineCmd
{
    RenderCmd base;
    V2 p0, p1;
    RGBA color2;
} LineCmd;

typedef struct s_RectCmd
{
    RenderCmd base;
    V2 p0, p1;
    UV uv0, uv1;
} RectCmd;

// Push commands
bool chk_push_clear(CmdList *cmd_list, RGBA color);
bool chk_push_line(CmdList *cmd_list, V2 p0, V2 p1, RGBA color);
bool chk_push_line_grad(CmdList *cmd_list, V2 p0, V2 p1, RGBA c0, RGBA c1);
bool chk_push_rect(CmdList *cmd_list, V2 p0, V2 p1, RGBA color);
bool chk_push_rect_tex(CmdList *cmd_list, V2 p0, V2 p1, RGBA color, Bitmap *bmp);
bool chk_push_rect_tex_uv(CmdList *cmd_list, V2 p0, V2 p1, RGBA color, Bitmap *bmp, UV uv0, UV uv1);
