#include "renderer.h"

// Generic Renderer related
bool chk_renderer_begin_frame(struct s_Renderer *base)
{
    bool result = false;
    if (base->begin_frame)
        result = base->begin_frame(base);
    return result;
}

void chk_renderer_end_frame(struct s_Renderer *base)
{
    if (base->end_frame)
        base->end_frame(base);
}

void chk_renderer_present_frame(struct s_Renderer *base)
{
    if (base->present_frame)
        base->present_frame(base);
}

void chk_renderer_process_cmds(struct s_Renderer *base, CmdList *cmd_list)
{
    if (base->process_cmds)
        base->process_cmds(base, cmd_list);
}

void chk_renderer_destroy(struct s_Renderer *base)
{
    if (base->cleanup)
        base->cleanup(base);
}

void chk_renderer_free(struct s_Renderer *base)
{
    chk_renderer_destroy(base);
    chk_free(base);
}

void chk_renderer_set_internal_resolution(struct s_Renderer *base, int w, int h)
{
    base->m_internal_resolution_is_set = true;
    base->m_internal_resolution_w = w;
    base->m_internal_resolution_h = h;
}

void chk_renderer_reset_internal_resolution(struct s_Renderer *base)
{
    base->m_internal_resolution_is_set = false;
    base->m_internal_resolution_w = base->window->fb_w;
    base->m_internal_resolution_h = base->window->fb_h;
}

// Null-Renderer related
bool chk_null_renderer_init(Renderer *base, NullRendererData *data, Window *window)
{
    base->name = "chk_null_renderer";
    base->data = data;
    base->window = window;

    base->begin_frame = chk_null_renderer_begin_frame;
    base->end_frame = chk_null_renderer_end_frame;
    base->present_frame = chk_null_renderer_present_frame;
    base->process_cmds = chk_null_renderer_process_cmds;
    base->cleanup = chk_null_renderer_cleanup;

    // Do something with w, h, win_w, win_h ...
    return true;
}

bool chk_null_renderer_begin_frame(struct s_Renderer *base)
{
    NullRendererData *renderer = base->data;
    // Do something with the renderer...
    return true;
}

void chk_null_renderer_end_frame(struct s_Renderer *base)
{
    NullRendererData *renderer = base->data;
    // Do something with the renderer...
}

void chk_null_renderer_present_frame(struct s_Renderer *base)
{
    NullRendererData *renderer = base->data;
    // Do something with the renderer...
}

void chk_null_renderer_process_cmds(struct s_Renderer *base, CmdList *cmd_list)
{
    NullRendererData *renderer = base->data;
    (void)cmd_list;
    // Do something with the renderer and the command list...
}

void chk_null_renderer_cleanup(struct s_Renderer *base)
{
    NullRendererData *renderer = base->data;
    // Do something with the renderer... For example, if we allocated memory, free it here.
}
