#pragma once

#include <common.h>
#include <window.h>

#include "cmd_list.h"

struct s_Renderer;
typedef void fn_RendererBeginFrame(struct s_Renderer *base);
typedef void fn_RendererEndFrame(struct s_Renderer *base);
typedef void fn_RendererPresentFrame(struct s_Renderer *base);
typedef void fn_RendererProcessCmds(struct s_Renderer *base, CmdList *cmd_list);
typedef void fn_RendererCleanup(struct s_Renderer *base);

typedef struct s_Renderer
{
    const char *name;
    Window *window;

    // State from current frame. READ ONLY AFTER CALLING chk_renderer_begin_frame(...)
    int m_internal_resolution_w, m_internal_resolution_h;
    int m_last_internal_resolution_w, m_last_internal_resolution_h;
    bool m_internal_resolution_is_set;

    // Generic Callbacks that will be filled by the specific renderer
    fn_RendererBeginFrame *begin_frame;
    fn_RendererEndFrame *end_frame;
    fn_RendererPresentFrame *present_frame;
    fn_RendererProcessCmds *process_cmds;
    fn_RendererCleanup *cleanup;

    // Normally the renderer's data pointer
    void *data;
} Renderer;

void chk_renderer_begin_frame(struct s_Renderer *base);
void chk_renderer_end_frame(struct s_Renderer *base);
void chk_renderer_present_frame(struct s_Renderer *base);
void chk_renderer_process_cmds(struct s_Renderer *base, CmdList *cmd_list);
void chk_renderer_destroy(struct s_Renderer *base);
void chk_renderer_free(struct s_Renderer *base);

void chk_renderer_set_internal_resolution(struct s_Renderer *base, int w, int h);
void chk_renderer_reset_internal_resolution(struct s_Renderer *base);

typedef struct s_NullRendererData
{
    int m_unused;
} NullRendererData;

bool chk_null_renderer_init(Renderer *base, NullRendererData *data, Window *window);

void chk_null_renderer_begin_frame(struct s_Renderer *base);
void chk_null_renderer_end_frame(struct s_Renderer *base);
void chk_null_renderer_present_frame(struct s_Renderer *base);
void chk_null_renderer_process_cmds(struct s_Renderer *base, CmdList *cmd_list);
void chk_null_renderer_cleanup(struct s_Renderer *base);
