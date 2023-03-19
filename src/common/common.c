#include "common.h"

#include <stdlib.h>
#include <string.h>
#include <whereami.h>

// Path related
static char *g_path_to_exe;
static size_t g_path_to_exe_length;
static char *g_exe_name;
static size_t g_exe_name_length;

const char *path_to_exe()
{
    static bool s_path_to_exe_computed = false;
    if (!s_path_to_exe_computed)
    {
        int dirname_length = 0;
        int length = wai_getExecutablePath(NULL, 0, &dirname_length);
        g_path_to_exe = chk_alloc(sizeof(char) * (length + 1));
        length = wai_getExecutablePath(g_path_to_exe, length + 1, &dirname_length);
        int exe_length = length - dirname_length - 1;
        g_exe_name = chk_alloc(sizeof(char) * (exe_length + 1));
        memcpy(g_exe_name, g_path_to_exe + dirname_length + 1, exe_length);
        g_exe_name[exe_length] = '\0';
        g_path_to_exe[dirname_length + 1] = '\0';

        g_path_to_exe_length = strlen(g_path_to_exe);
        g_exe_name_length = strlen(g_exe_name);

        s_path_to_exe_computed = true;
    }

    return g_path_to_exe;
}

size_t path_to(const char *rel_path, char *out_buffer, size_t out_buffer_length)
{
    const char *exe_path = path_to_exe();
    size_t rel_path_length = strlen(rel_path);
    size_t needed = g_path_to_exe_length + rel_path_length;

    if (!out_buffer || out_buffer_length < needed)
        return needed;

    memcpy(out_buffer, exe_path, g_path_to_exe_length);
    memcpy(out_buffer + g_path_to_exe_length, rel_path, rel_path_length + 1);
    return needed;
}

// Memory related
void *chk_alloc(size_t amount) { return malloc(amount); }
void *chk_zalloc(size_t amount) { return calloc(1, amount); }
void chk_free(void *ptr) { free(ptr); }
void chk_zero_memory(void *ptr, size_t amount) { memset(ptr, 0, amount); }
