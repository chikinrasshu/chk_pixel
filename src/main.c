#include <bitmap.h>
#include <cmd_list.h>
#include <common.h>
#include <file_io.h>
#include <renderer.h>
#include <soft.h>
#include <window.h>
#include <math/algebra.h>

#include <spng.h>

typedef struct s_Engine
{
    MemoryArena main_arena;
    MemoryArena temp_arena;

    // Subsystems
    Window *window;
    Renderer *renderer;
    CmdList *cmd_list;
} Engine;

void chk_engine_destroy(Engine *engine)
{
    chk_cmd_list_destroy(engine->cmd_list);
    chk_renderer_destroy(engine->renderer);
    chk_window_destroy(engine->window);

    chk_memory_arena_destroy(&engine->temp_arena);
    chk_memory_arena_destroy(&engine->main_arena);
}

#define unpack_engine(UserPtr)             \
    Engine *engine = (Engine *)(UserPtr);  \
    Window *window = engine->window;       \
    Renderer *renderer = engine->renderer; \
    CmdList *cmd_list = engine->cmd_list;

void cb_main(void *user_ptr)
{
    unpack_engine(user_ptr);

    if (chk_renderer_begin_frame(renderer))
    {
        // Do the rendering here...
        chk_cmd_list_reset(cmd_list);
        chk_push_clear(cmd_list, v4(0.7f, 0.8f, 0.9f, 1.0f));

        V2 mouse_pos = chk_renderer_get_mouse_pos(renderer);

        RGBA color_blue = v4_div(v4(42, 75, 215, 255), 255.0f);
        Bitmap white_tex = {0};
        chk_push_rect_tex(cmd_list, v2(4, 8), mouse_pos, color_blue, &white_tex);

        // End the frame
        chk_renderer_process_cmds(renderer, cmd_list);
        chk_renderer_end_frame(renderer);
        chk_renderer_present_frame(renderer);
    }
}

int main()
{
    Engine engine = {0};

    // Memory
    chk_error_if(!chk_memory_arena_zalloc(&engine.main_arena, chk_megabytes(256)),
                 "Failed to allocate the main arena.") return 1;
    chk_error_if(!chk_memory_arena_zalloc(&engine.temp_arena, chk_gigabytes(1)),
                 "Failed to allocate the transient arena.") return 1;

    // Window
    engine.window = chk_memory_arena_push_struct(&engine.main_arena, Window);
    engine.window->user_ptr = &engine;
    engine.window->user_callback = cb_main;
    chk_error_if(!chk_window_init_opengl(engine.window, 800, 600, "chk_pixel"),
                 "Failed to initialize the main OpenGL Window.") return 1;

    // Renderer
    engine.renderer = chk_memory_arena_push_struct(&engine.main_arena, Renderer);
    SoftRendererData *soft_data = chk_memory_arena_push_struct(&engine.main_arena, SoftRendererData);
    chk_renderer_set_internal_resolution(engine.renderer, 80, 60);
    chk_error_if(!chk_soft_renderer_init(engine.renderer, soft_data, engine.window),
                 "Failed to initialize the Soft Renderer.") return 1;

    // Command List
    engine.cmd_list = chk_memory_arena_push_struct(&engine.main_arena, CmdList);
    size_t cmd_list_size = chk_megabytes(256);
    void *cmd_list_memory = chk_memory_arena_zpush(&engine.temp_arena, cmd_list_size);
    chk_error_if(!chk_cmd_list_init(engine.cmd_list, cmd_list_memory, cmd_list_size),
                 "Failed to create the Render Command List.") return 1;

    // Textures
    char path_buffer[1024];
    path_to("data/pixel_alphabet.png", path_buffer, chk_array_count(path_buffer));
    size_t raw_png_buffer_size = chk_get_file_size(path_buffer);
    void *raw_png_buffer = chk_memory_arena_push(&engine.temp_arena, raw_png_buffer_size);
    chk_read_binary_file(path_buffer, raw_png_buffer, raw_png_buffer_size);

    // Decode PNG
    Bitmap png_image = {0};
    {
        spng_ctx *ctx = spng_ctx_new(0);
        chk_error_if(!ctx, "Failed to create the sPNG context.") return 1;

        spng_set_png_buffer(ctx, raw_png_buffer, raw_png_buffer_size);
        spng_decode_chunks(ctx);

        size_t png_size;
        spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &png_size);

        struct spng_ihdr png_header = {0};
        chk_error_if(spng_get_ihdr(ctx, &png_header), "Failed to get the iHDR header.") return 1;

        void *png_memory = chk_memory_arena_push(&engine.main_arena, png_size);
        chk_errorf_if(!png_memory, "Failed to allocate %zu bytes for the decoded PNG file.", png_size) return 1;
        spng_decode_image(ctx, png_memory, png_size, SPNG_FMT_RGBA8, 0);
        spng_ctx_free(ctx);

        png_image.w = png_header.width;
        png_image.h = png_header.height;
        png_image.bpp = 4;
        png_image.size = png_size;
        png_image.stride = png_header.width;
        png_image.memory = png_memory;
        png_image.memory_size = png_size;
        png_image.owns_memory = true;
    }

    // Run the program
    int error_code = chk_window_run(engine.window);
    chk_errorf_if(error_code != 0, "Window exited with an error: %d", error_code) return 1;

    // Cleanup
    chk_engine_destroy(&engine);
    return error_code;
}
