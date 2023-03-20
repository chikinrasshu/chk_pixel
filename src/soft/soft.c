#include "soft.h"

V2 scale_keep_aspect(V2 src, V2 dst)
{
    V2 s;
    float src_ratio = src.x / src.y;
    float dst_ratio = dst.x / dst.y;
    float scale = 1.0f;

    if (src_ratio == dst_ratio)
    {
        scale = dst.x / src.x;
    }
    else
    {
        if (src_ratio < dst_ratio)
            scale = dst.y / src.y;
        else
            scale = dst.x / src.x;
    }

    s.x = src.x * scale;
    s.y = src.y * scale;
    return s;
}

bool chk_soft_renderer_init(Renderer *base, SoftRendererData *data, Window *window)
{
    base->name = "chk_soft_renderer";
    base->data = data;
    base->window = window;

    base->begin_frame = chk_soft_renderer_begin_frame;
    base->end_frame = chk_soft_renderer_end_frame;
    base->present_frame = chk_soft_renderer_present_frame;
    base->process_cmds = chk_soft_renderer_process_cmds;
    base->cleanup = chk_soft_renderer_cleanup;

    // Init data
    int w = base->m_internal_resolution_w, h = base->m_internal_resolution_h;
    if (w <= 0)
        w = base->window->fb_w;
    if (h <= 0)
        h = base->window->fb_h;

    // Allocate enough memory for 2x the screen resolution
    // int max_w = window->virtual_rect.w, max_h = window->virtual_rect.h;
    int max_w = 2 * 2560; // HARDCODED while i think about how to merge all monitors into a single value
    int max_h = 2 * 1440;

    int bmp_bpp = 4;
    size_t bmp_memory_size = chk_bitmap_query(max_w, max_h, bmp_bpp);
    void *bmp_memory = chk_alloc(bmp_memory_size);
    chk_error_if(!bmp_memory, "Failed to allocate enough memory to fit a bitmap the size of the whole screen.") return false;

    data->target.owns_memory = true;
    chk_bitmap_init(&data->target, max_w, max_h, bmp_bpp, bmp_memory, bmp_memory_size);

    // Init OpenGL
    chk_soft_gl_init(&data->gl, window->fb_w, window->fb_h);
    return true;
}

bool chk_soft_renderer_begin_frame(Renderer *base)
{
    SoftRendererData *data = base->data;

    // Resize the render target
    if (!base->m_internal_resolution_is_set && base->window->fb_size_changed && !base->window->is_minimized)
    {
        chk_error_if(!chk_bitmap_resize(&data->target, base->window->fb_w, base->window->fb_h),
                     "Failed to resize the render target.") return false;
    }

    // Set the new viewport if the target covers the whole window.
    if (base->window->fb_size_changed && !base->window->is_minimized)
    {
        chk_soft_gl_update_viewport(&data->gl, 0, 0, base->window->fb_w, base->window->fb_h);
    }

    // Otherwise, clear the whole window, then set the viewport back to only the scaled rect.
    if (base->m_internal_resolution_is_set)
    {
        if (data->target.w != base->m_internal_resolution_w || data->target.h != base->m_internal_resolution_h)
        {
            int new_w = base->m_internal_resolution_w, new_h = base->m_internal_resolution_h;
            chk_errorf_if(!chk_bitmap_resize(&data->target, new_w, new_h),
                          "Failed to resize the render target. Requested size: %d x %d", new_w, new_h) return false;
        }

        chk_soft_gl_clear_region(&data->gl, 0, 0, base->window->fb_w, base->window->fb_h);

        V2 scaled = scale_keep_aspect(v2((float)data->target.w, (float)data->target.h),
                                      v2((float)base->window->fb_w, (float)base->window->fb_h));

        int scaled_x = (int)((base->window->fb_w - scaled.x) / 2.0f);
        int scaled_y = (int)((base->window->fb_h - scaled.y) / 2.0f);

        chk_soft_gl_update_viewport(&data->gl, scaled_x, scaled_y, (int)scaled.w, (int)scaled.h);
    }

    return true;
}

void chk_soft_renderer_end_frame(Renderer *base)
{
    SoftRendererData *data = base->data;
    (void)data;
    // In the software renderer we have to do nothing here...
}

void chk_soft_renderer_present_frame(Renderer *base)
{
    SoftRendererData *data = base->data;
    chk_soft_gl_present(&data->gl, &data->target);
}

void chk_soft_renderer_process_cmds(Renderer *base, CmdList *cmd_list)
{
    SoftRendererData *data = base->data;

    RenderCmd *raw_cmd = cmd_list->arena.memory;
    if (cmd_list->count > 0)
    {
        size_t cmd_count = 0;
        while (raw_cmd)
        {
            switch (raw_cmd->kind)
            {
            default:
            case CmdKind_Unknown:
                raw_cmd = NULL;
                break;

            case CmdKind_Clear:
                chk_soft_renderer_draw_clear(base, (ClearCmd *)raw_cmd);
                break;

            case CmdKind_Line:
                chk_soft_renderer_draw_line(base, (LineCmd *)raw_cmd);
                break;

            case CmdKind_Rect:
                chk_soft_renderer_draw_rect(base, (RectCmd *)raw_cmd);
                break;
            }

            raw_cmd = raw_cmd->next;
            ++cmd_count;
        }
    }
}

void chk_soft_renderer_cleanup(Renderer *base)
{
    SoftRendererData *data = base->data;

    chk_bitmap_destroy(&data->target);
    chk_soft_gl_destroy(&data->gl);
}

// Software renderer goes here...
void chk_soft_renderer_draw_clear(Renderer *base, ClearCmd *cmd)
{
    SoftRendererData *data = base->data;
    Bitmap *target = &data->target;
    uint32_t packed_color = chk_packed_rgba(cmd->base.color);

    uint8_t *row = (uint8_t *)target->memory;
    for (int y = 0; y < target->h; ++y)
    {
        uint32_t *pixel = (uint32_t *)row;
        for (int x = 0; x < target->w; ++x)
        {
            *pixel++ = packed_color;
        }
        row += target->stride;
    }
}

void chk_soft_renderer_draw_line(Renderer *base, LineCmd *cmd)
{
    /* No-Op */
}

void chk_soft_renderer_draw_rect(Renderer *base, RectCmd *cmd)
{
    /* No-Op */
}