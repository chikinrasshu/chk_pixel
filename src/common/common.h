#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// Helpers for debugging
#ifndef CHK_SRC_DIR_LENGTH
#define CHK_SRC_DIR_LENGTH 0ULL
#endif
#define CHK_FILE (&__FILE__[CHK_SRC_DIR_LENGTH])

// Helpers for memory sizes
#define chk_kilobytes(n) ((n)*1024ULL)
#define chk_megabytes(n) (chk_kilobytes(n) * 1024ULL)
#define chk_gigabytes(n) (chk_megabytes(n) * 1024ULL)
#define chk_terabytes(n) (chk_gigabytes(n) * 1024ULL)

#define chk_array_count(a) (sizeof(a) / sizeof((a)[0]))

bool chk_error_print(const char *func, const char *file, size_t line, const char *msg, ...);
#define chk_error_if(Cond, Msg) if ((Cond) && chk_error_print(__func__, CHK_FILE, __LINE__, "%s", (Msg)))
#define chk_errorf_if(Cond, Msg, ...) if ((Cond) && chk_error_print(__func__, CHK_FILE, __LINE__, (Msg), __VA_ARGS__))

bool chk_log_print(const char *func, const char *file, size_t line, const char *msg, ...);

#define chk_log_if(Cond, Msg)                                         \
    do                                                                \
    {                                                                 \
        if ((Cond))                                                   \
            chk_log_print(__func__, CHK_FILE, __LINE__, "%s", (Msg)); \
    } while (0)

#define chk_logf_if(Cond, Msg, ...)                                          \
    do                                                                       \
    {                                                                        \
        if ((Cond))                                                          \
            chk_log_print(__func__, CHK_FILE, __LINE__, (Msg), __VA_ARGS__); \
    } while (0)

// Path helpers
const char *path_to_exe();
size_t path_to(const char *rel_path, char *out_buffer, size_t out_buffer_length);

// Memory helpers
void *chk_alloc(size_t amount);
void *chk_zalloc(size_t amount);
void chk_free(void *ptr);

void chk_zero_memory(void *ptr, size_t amount);
void chk_copy_memory(void *dst, void *src, size_t amount);

// Logic helpers
#define chk_swap(x, y)                                                                   \
    do                                                                                   \
    {                                                                                    \
        unsigned char chk_swap_temp_zz[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
        chk_copy_memory(chk_swap_temp_zz, &y, sizeof(x));                                \
        chk_copy_memory(&y, &x, sizeof(x));                                              \
        chk_copy_memory(&x, chk_swap_temp_zz, sizeof(x));                                \
    } while (0)
