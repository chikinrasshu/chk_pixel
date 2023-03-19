#pragma once

#include <common.h>

#define CHK_WINDOW_NO_CONTEXT 0
#define CHK_WINDOW_OPENGL_CONTEXT 1
#define CHK_WINDOW_VULKAN_CONTEXT 2

typedef void cb_WindowCallback(void *);
struct GLFWwindow;

typedef struct s_Window
{
    // State (Read only)
    bool is_running;
    int x, y, w, h, fb_w, fb_h;
    bool is_maximized, is_minimized, is_fullscreen, is_focused;
    double dpi_x, dpi_y;
    float mouse_x, mouse_y, mouse_dx, mouse_dy, mouse_wx, mouse_wy;

    // Trackers (Read only)
    bool pos_changed, size_changed, fb_size_changed, dpi_changed;
    bool maximized_changed, minimized_changed, fullscreen_changed;
    bool focus_changed;

    // Callback related
    void *user_ptr;
    cb_WindowCallback *user_callback;

    // Timing related
    double current_time, last_time, delta_time, delta_ms;
    uint64_t frame_count;

    // Internals
    struct GLFWwindow *m_handle;
    bool uses_opengl;
} Window;

Window *chk_window_create(int w, int h, const char *caption);
Window *chk_window_create_opengl(int w, int h, const char *caption);
Window *chk_window_create_ex(int w, int h, const char *caption, int with_context);
bool chk_window_free(Window *win);

bool chk_window_init(Window *win, int w, int h, const char *caption);
bool chk_window_init_opengl(Window *win, int w, int h, const char *caption);
bool chk_window_init_ex(Window *win, int w, int h, const char *caption, int with_context);
bool chk_window_destroy(Window *win);

bool chk_window_show(Window *win);
int chk_window_run(Window *win);
bool chk_window_process_frame(Window *win);
