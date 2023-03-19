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

void chk_soft_renderer_begin_frame(struct s_Renderer *base);
void chk_soft_renderer_end_frame(struct s_Renderer *base);
void chk_soft_renderer_present_frame(struct s_Renderer *base);
void chk_soft_renderer_process_cmds(struct s_Renderer *base, CmdList *cmd_list);
void chk_soft_renderer_cleanup(struct s_Renderer *base);
