#include "window.h"

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <stdlib.h>

/* GLFW State Management */
static int g_chk_internal_window_count = 0;
bool chk_internal_register_window();
bool chk_internal_release_window();

/* GLFW Callbacks Fwd */
void chk_cb_error(int error, const char *description) { fprintf(stderr, "GLFW Error: %s\n", description); }
void chk_cb_keyfun(GLFWwindow *handle, int key, int scancode, int action, int mods);
void chk_cb_charfun(GLFWwindow *handle, unsigned int codepoint);
void chk_cb_charmodsfun(GLFWwindow *handle, unsigned int codepoint, int mods);
void chk_cb_dropfun(GLFWwindow *handle, int path_count, const char **paths);
void chk_cb_cursorposfun(GLFWwindow *handle, double xpos, double ypos);
void chk_cb_cursorenterfun(GLFWwindow *handle, int entered);
void chk_cb_mousebuttonfun(GLFWwindow *handle, int button, int action, int mods);
void chk_cb_scrollfun(GLFWwindow *handle, double dx, double dy);
void chk_cb_windowposfun(GLFWwindow *handle, int x, int y);
void chk_cb_windowsizefun(GLFWwindow *handle, int width, int height);
void chk_cb_framebuffersizefun(GLFWwindow *handle, int width, int height);
void chk_cb_windowrefreshfun(GLFWwindow *handle);
void chk_cb_windowclosefun(GLFWwindow *handle);
void chk_cb_windowiconifyfun(GLFWwindow *handle, int minimized);
void chk_cb_windowmaximizefun(GLFWwindow *handle, int maximized);
void chk_cb_windowfocusfun(GLFWwindow *handle, int focused);
void chk_cb_windowcontentscalefun(GLFWwindow *handle, float dpi_x, float dpi_y);

Window *chk_window_create(int w, int h, const char *caption) { return chk_window_create_ex(w, h, caption, CHK_WINDOW_NO_CONTEXT); }
Window *chk_window_create_opengl(int w, int h, const char *caption) { return chk_window_create_ex(w, h, caption, CHK_WINDOW_OPENGL_CONTEXT); }
Window *chk_window_create_ex(int w, int h, const char *caption, int with_context)
{
    Window *win = malloc(sizeof(*win));
    if (!win)
    {
        fprintf(stderr, "chk_window_create_ex: Failed to allocate memory for the window.\n");
        return NULL;
    }

    if (!chk_window_init_ex(win, w, h, caption, with_context))
    {
        free(win);
        return NULL;
    }
    return win;
}

bool chk_window_free(Window *win)
{
    bool destroyed = chk_window_destroy(win);
    free(win);
    return destroyed;
}

bool chk_window_init(Window *win, int w, int h, const char *caption) { return chk_window_init_ex(win, w, h, caption, CHK_WINDOW_NO_CONTEXT); }
bool chk_window_init_opengl(Window *win, int w, int h, const char *caption) { return chk_window_init_ex(win, w, h, caption, CHK_WINDOW_OPENGL_CONTEXT); }
bool chk_window_init_ex(Window *win, int w, int h, const char *caption, int with_context)
{
    if (!chk_internal_register_window())
        return false;

    if (!win)
    {
        fprintf(stderr, "chk_window_init_ex: win was NULL.\n");
        chk_internal_release_window();
        return false;
    }

    if (with_context & CHK_WINDOW_NO_CONTEXT)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }
    else if (with_context & CHK_WINDOW_OPENGL_CONTEXT)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        win->uses_opengl = true;
    }

    win->w = w;
    win->h = h;
    win->m_handle = glfwCreateWindow(w, h, caption, NULL, NULL);
    if (!win->m_handle)
    {
        if (!chk_internal_release_window())
            return false;

        fprintf(stderr, "chk_window_init_ex: Failed to create the main window.\n");
        return false;
    }

    glfwSetWindowUserPointer(win->m_handle, win);

    glfwSetKeyCallback(win->m_handle, chk_cb_keyfun);
    glfwSetCharCallback(win->m_handle, chk_cb_charfun);
    glfwSetCharModsCallback(win->m_handle, chk_cb_charmodsfun);
    glfwSetDropCallback(win->m_handle, chk_cb_dropfun);
    glfwSetCursorPosCallback(win->m_handle, chk_cb_cursorposfun);
    glfwSetCursorEnterCallback(win->m_handle, chk_cb_cursorenterfun);
    glfwSetMouseButtonCallback(win->m_handle, chk_cb_mousebuttonfun);
    glfwSetScrollCallback(win->m_handle, chk_cb_scrollfun);
    glfwSetWindowPosCallback(win->m_handle, chk_cb_windowposfun);
    glfwSetWindowSizeCallback(win->m_handle, chk_cb_windowsizefun);
    glfwSetFramebufferSizeCallback(win->m_handle, chk_cb_framebuffersizefun);
    glfwSetWindowRefreshCallback(win->m_handle, chk_cb_windowrefreshfun);
    glfwSetWindowCloseCallback(win->m_handle, chk_cb_windowclosefun);
    glfwSetWindowIconifyCallback(win->m_handle, chk_cb_windowiconifyfun);
    glfwSetWindowMaximizeCallback(win->m_handle, chk_cb_windowmaximizefun);
    glfwSetWindowFocusCallback(win->m_handle, chk_cb_windowfocusfun);
    glfwSetWindowContentScaleCallback(win->m_handle, chk_cb_windowcontentscalefun);

    glfwSetWindowSizeLimits(win->m_handle, 100, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwGetWindowPos(win->m_handle, &win->x, &win->y);
    glfwGetWindowSize(win->m_handle, &win->w, &win->h);
    glfwGetFramebufferSize(win->m_handle, &win->fb_w, &win->fb_h);

    float dpi_x, dpi_y;
    glfwGetWindowContentScale(win->m_handle, &dpi_x, &dpi_y);
    win->dpi_x = dpi_x;
    win->dpi_y = dpi_y;

    if (win->uses_opengl)
    {
        glfwMakeContextCurrent(win->m_handle);
        gladLoadGL(glfwGetProcAddress);
        glfwSwapInterval(1);
    }

    win->is_running = true;
    return win->is_running;
}

bool chk_window_destroy(Window *win)
{
    return chk_internal_release_window();
}

bool chk_window_show(Window *win)
{
    glfwShowWindow(win->m_handle);
    return true;
}

bool chk_window_process_frame(Window *win)
{
    if (!win)
    {
        fprintf(stderr, "chk_window_process_frame: win was NULL.\n");
        return false;
    }

    // Calculate the delta time
    win->current_time = glfwGetTime();
    win->delta_time = win->current_time - win->last_time;
    win->delta_ms = win->delta_time * 1000.0;
    win->last_time = win->current_time;

    // Call the user callback
    if (win->user_callback)
        win->user_callback(win->user_ptr);

    // Clear the trackers
    win->pos_changed = win->size_changed = win->fb_size_changed = win->dpi_changed = false;
    win->maximized_changed = win->minimized_changed = win->fullscreen_changed = win->focus_changed = false;
    win->mouse_wx = win->mouse_wy = 0.0;

    // Swap the window's buffer if using OpenGL
    if (win->uses_opengl)
        glfwSwapBuffers(win->m_handle);

    ++win->frame_count;
    return win->is_running;
}

int chk_window_run(Window *win)
{
    if (!win)
    {
        fprintf(stderr, "chk_window_run: win was NULL.\n");
        return -1;
    }

    if (!chk_window_show(win))
    {
        return -1;
    }

    glfwSetTime(0.0);
    win->current_time = win->last_time = glfwGetTime();
    while (win->is_running)
    {
        // Process the window events
        glfwPollEvents();
        if (!chk_window_process_frame(win))
            break;
    }

    return 0;
}

/* GLFW State Management */
bool chk_internal_register_window()
{
    if (!g_chk_internal_window_count && !glfwInit())
        return false;
    glfwSetErrorCallback(chk_cb_error);

    ++g_chk_internal_window_count;
    return true;
}

bool chk_internal_release_window()
{
    --g_chk_internal_window_count;

    if (!g_chk_internal_window_count)
    {
        glfwTerminate();
    }
    else if (g_chk_internal_window_count < 0)
    {
        fprintf(stderr, "chk_internal_release_window: Trying to release with registration count '%d'.\n", g_chk_internal_window_count);
        return false;
    }
    return true;
}

/* GLFW Callbacks Implementation */
#define CHK_GET_WIN_PTR                             \
    Window *win = glfwGetWindowUserPointer(handle); \
    assert(win != NULL);
void chk_cb_keyfun(GLFWwindow *handle, int key, int scancode, int action, int mods)
{
    CHK_GET_WIN_PTR;
}

void chk_cb_charfun(GLFWwindow *handle, unsigned int codepoint)
{
    CHK_GET_WIN_PTR;
}

void chk_cb_charmodsfun(GLFWwindow *handle, unsigned int codepoint, int mods)
{
    CHK_GET_WIN_PTR;
}

void chk_cb_dropfun(GLFWwindow *handle, int path_count, const char **paths)
{
    CHK_GET_WIN_PTR;
}

void chk_cb_cursorposfun(GLFWwindow *handle, double xpos, double ypos)
{
    CHK_GET_WIN_PTR;
    xpos *= win->dpi_x;
    ypos *= win->dpi_y;

    win->mouse_dx = (float)xpos - win->mouse_x;
    win->mouse_dy = (float)ypos - win->mouse_y;
    win->mouse_x = (float)xpos;
    win->mouse_y = (float)ypos;
}

void chk_cb_cursorenterfun(GLFWwindow *handle, int entered)
{
    CHK_GET_WIN_PTR;
}

void chk_cb_mousebuttonfun(GLFWwindow *handle, int button, int action, int mods)
{
    CHK_GET_WIN_PTR;
}

void chk_cb_scrollfun(GLFWwindow *handle, double dx, double dy)
{
    CHK_GET_WIN_PTR;
    win->mouse_wx += (float)dx;
    win->mouse_wy += (float)dy;
}

void chk_cb_windowposfun(GLFWwindow *handle, int x, int y)
{
    CHK_GET_WIN_PTR;
    win->pos_changed = true;
    win->x = x;
    win->y = y;
}

void chk_cb_windowsizefun(GLFWwindow *handle, int width, int height)
{
    CHK_GET_WIN_PTR;
    win->size_changed = true;
    win->w = width;
    win->h = height;
}

void chk_cb_framebuffersizefun(GLFWwindow *handle, int width, int height)
{
    CHK_GET_WIN_PTR;
    win->fb_size_changed = true;
    win->fb_w = width;
    win->fb_h = height;
}

void chk_cb_windowrefreshfun(GLFWwindow *handle)
{
    CHK_GET_WIN_PTR;
    chk_window_process_frame(win);
}

void chk_cb_windowclosefun(GLFWwindow *handle)
{
    CHK_GET_WIN_PTR;
    win->is_running = false;
}

void chk_cb_windowiconifyfun(GLFWwindow *handle, int minimized)
{
    CHK_GET_WIN_PTR;
    win->minimized_changed = true;
    win->is_minimized = minimized;
}

void chk_cb_windowmaximizefun(GLFWwindow *handle, int maximized)
{
    CHK_GET_WIN_PTR;
    win->maximized_changed = true;
    win->is_maximized = maximized;
}

void chk_cb_windowfocusfun(GLFWwindow *handle, int focused)
{
    CHK_GET_WIN_PTR;
    win->focus_changed = true;
    win->is_focused = focused;
}

void chk_cb_windowcontentscalefun(GLFWwindow *handle, float dpi_x, float dpi_y)
{
    CHK_GET_WIN_PTR;
    win->dpi_changed = true;
    win->dpi_x = dpi_x;
    win->dpi_y = dpi_y;
}

#undef CHK_GET_WIN_PTR