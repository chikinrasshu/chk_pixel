#include "bitmap.h"
#include "cmd_list.h"
#include "common.h"
#include "renderer.h"
#include "soft.h"
#include "window.h"

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

        // End the frame
        chk_renderer_process_cmds(renderer, cmd_list);
        chk_renderer_end_frame(renderer);
        chk_renderer_present_frame(renderer);
    }
}

int main()
{
    Engine engine = {0};

    chk_error_if(!chk_memory_arena_alloc(&engine.main_arena, chk_megabytes(256)), "Failed to allocate the main arena.") return 1;
    chk_error_if(!chk_memory_arena_alloc(&engine.temp_arena, chk_gigabytes(1)), "Failed to allocate the transient arena.") return 1;

    engine.window = chk_memory_arena_push_struct(&engine.main_arena, Window);
    engine.window->user_ptr = &engine;
    engine.window->user_callback = cb_main;
    chk_window_init_opengl(engine.window, 800, 600, "chk_pixel");

    engine.renderer = chk_memory_arena_push_struct(&engine.main_arena, Renderer);
    SoftRendererData *soft_data = chk_memory_arena_push_struct(&engine.main_arena, SoftRendererData);
    chk_renderer_set_internal_resolution(engine.renderer, 80, 60);
    chk_soft_renderer_init(engine.renderer, soft_data, engine.window);

    engine.cmd_list = chk_memory_arena_push_struct(&engine.main_arena, CmdList);
    size_t cmd_list_size = chk_megabytes(256);
    void *cmd_list_memory = chk_memory_arena_push(&engine.temp_arena, cmd_list_size);
    chk_cmd_list_init(engine.cmd_list, cmd_list_memory, cmd_list_size);

    // Run the program
    chk_window_run(engine.window);

    // Cleanup
    chk_engine_destroy(&engine);
    return 0;
}
