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

    data->target.bpp = 4;
    data->target.memory_size = chk_bitmap_init(NULL, w, h, data->target.bpp);
    data->target.memory = chk_alloc(data->target.memory_size);
    if (!data->target.memory)
    {
        fprintf(stderr, "chk_soft_renderer_init: Failed to allocate %zu bytes for the render target!\n", data->target.memory_size);
        return false;
    }
    chk_bitmap_resize(&data->target, w, h);

    // Init OpenGL
    chk_soft_gl_init(&data->gl, window->fb_w, window->fb_h);
    return true;
}

void chk_soft_renderer_begin_frame(struct s_Renderer *base)
{
    SoftRendererData *data = base->data;

    // Resize the render target
    if (!base->m_internal_resolution_is_set && base->window->fb_size_changed && !base->window->is_minimized)
    {
        chk_bitmap_resize(&data->target, base->window->fb_w, base->window->fb_h);
    }

    // Set the new viewport if the target covers the whole window.
    if (base->window->fb_size_changed && !base->window->is_minimized)
    {
        chk_soft_gl_update_viewport(&data->gl, 0, 0, base->window->fb_w, base->window->fb_h);
    }

    // Otherwise, clear the whole window, then set the viewport back to only the scaled rect.
    if (base->m_internal_resolution_is_set)
    {
        chk_soft_gl_clear_region(&data->gl, 0, 0, base->window->fb_w, base->window->fb_h);

        V2 scaled = scale_keep_aspect(v2((float)data->target.w, (float)data->target.h),
                                      v2((float)base->window->fb_w, (float)base->window->fb_h));

        int scaled_x = (int)((base->window->fb_w - scaled.x) / 2.0f);
        int scaled_y = (int)((base->window->fb_h - scaled.y) / 2.0f);

        chk_soft_gl_update_viewport(&data->gl, scaled_x, scaled_y, (int)scaled.w, (int)scaled.h);
    }
}

void chk_soft_renderer_end_frame(struct s_Renderer *base)
{
    SoftRendererData *data = base->data;
    (void)data;
    // In the software renderer we have to do nothing here...
}

void chk_soft_renderer_present_frame(struct s_Renderer *base)
{
    SoftRendererData *data = base->data;
    chk_soft_gl_present(&data->gl, &data->target);
}

void chk_soft_renderer_process_cmds(struct s_Renderer *base, CmdList *cmd_list)
{
    SoftRendererData *data = base->data;

    // Render the test pattern in the bitmap
    uint8_t *row = data->target.memory;
    for (int y = 0; y < data->target.h; ++y)
    {
        uint32_t *pixel = (uint32_t *)row;
        for (int x = 0; x < data->target.w; ++x)
        {
            *pixel++ = chk_packed_rgba_from_u8(x, y, 0, 255);
        }
        row += data->target.stride;
    }

    (void)data;
    (void)cmd_list;
}

void chk_soft_renderer_cleanup(struct s_Renderer *base)
{
    SoftRendererData *data = base->data;

    chk_bitmap_destroy(&data->target);
    if (data->target.memory)
        chk_free(data->target.memory);

    chk_soft_gl_destroy(&data->gl);
}
