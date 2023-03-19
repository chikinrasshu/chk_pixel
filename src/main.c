#include "bitmap.h"
#include "cmd_list.h"
#include "common.h"
#include "renderer.h"
#include "soft.h"
#include "window.h"

typedef struct s_Engine
{
    Window window;
    Renderer renderer;
    CmdList cmd_list;
} Engine;

void chk_engine_destroy(Engine *engine)
{
    chk_cmd_list_destroy(&engine->cmd_list);
    chk_renderer_destroy(&engine->renderer);
    chk_window_destroy(&engine->window);
}

// Main program
#define CHK_ENGINE_UNPACK(UserPtr)          \
    Engine *engine = UserPtr;               \
    Window *window = &engine->window;       \
    Renderer *renderer = &engine->renderer; \
    CmdList *cmd_list = &engine->cmd_list;

void cb_window_frame(void *user_ptr)
{
    CHK_ENGINE_UNPACK(user_ptr);

    chk_renderer_begin_frame(renderer);
    chk_renderer_process_cmds(renderer, cmd_list);
    chk_renderer_end_frame(renderer);
    chk_renderer_present_frame(renderer);
}

int main()
{
    Engine engine = {0};

    // Window init
    chk_window_init_opengl(&engine.window, 800, 600, "chk_pixel");

    // Renderer init
    SoftRendererData renderer_data = {0};
    chk_renderer_set_internal_resolution(&engine.renderer, 80, 60);
    chk_soft_renderer_init(&engine.renderer, &renderer_data, &engine.window);

    // Command list
    chk_cmd_list_init(&engine.cmd_list, chk_megabytes(256));

    // Run the program
    engine.window.user_callback = cb_window_frame;
    engine.window.user_ptr = &engine;
    chk_window_run(&engine.window);

    // Cleanup
    chk_engine_destroy(&engine);
}
