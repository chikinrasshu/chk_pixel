#pragma once

#include <bitmap.h>
#include <cmd_list.h>
#include <common.h>
#include <math/vec2.h>
#include <renderer.h>

#include "soft_gl.h"

typedef struct s_SoftRendererData
{
    Bitmap target;
    SoftGL gl;
} SoftRendererData;

bool chk_soft_renderer_init(Renderer *base, SoftRendererData *data, Window *window);

bool chk_soft_renderer_begin_frame(Renderer *base);
void chk_soft_renderer_end_frame(Renderer *base);
void chk_soft_renderer_present_frame(Renderer *base);
void chk_soft_renderer_process_cmds(Renderer *base, CmdList *cmd_list);
void chk_soft_renderer_cleanup(Renderer *base);

// Software rendering
void chk_soft_renderer_draw_clear(Renderer *base, ClearCmd *cmd);
void chk_soft_renderer_draw_line(Renderer *base, LineCmd *cmd);
void chk_soft_renderer_draw_rect(Renderer *base, RectCmd *cmd);