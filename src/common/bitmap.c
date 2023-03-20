#include "bitmap.h"

size_t chk_bitmap_query_stride(int w, int bpp)
{
    size_t line = (size_t)w * (size_t)(bpp * 8);
    size_t stride = (line + 31) / 32;
    return stride * 4;
}
size_t chk_bitmap_query(int w, int h, int bpp) { return (chk_bitmap_query_stride(w, bpp) * 4) * (size_t)h; }

bool chk_bitmap_init(Bitmap *bitmap, int w, int h, int bpp, void *memory, size_t memory_size)
{
    chk_error_if(!bitmap, "bitmap was NULL.") return false;
    chk_error_if(!memory, "memory was NULL.") return false;
    chk_error_if(!memory_size, "memory_size was zero.") return false;
    chk_error_if(w == 0 || h == 0, "A bitmap dimension was zero.") return false;
    chk_error_if(bpp <= 0, "bpp was zero.") return false;

    size_t required = chk_bitmap_query(w, h, bpp);
    chk_error_if(memory_size < required, "The passed buffer is too small.") return false;

    bitmap->w = w;
    bitmap->h = h;
    bitmap->bpp = bpp;
    bitmap->memory = memory;
    bitmap->memory_size = memory_size;
    bitmap->size = required;
    bitmap->stride = chk_bitmap_query_stride(w, bpp);

    printf("Bitmap dump: %d x %d at %d bpp.\n", bitmap->w, bitmap->h, bitmap->bpp);

    return true;
}

void chk_bitmap_destroy(Bitmap *bitmap)
{
    chk_error_if(!bitmap, "bitmap was NULL.") return;

    if (bitmap->owns_memory)
        chk_free(bitmap->memory);

    *bitmap = (Bitmap){0};
}

#include "colors.h"
bool chk_bitmap_resize(Bitmap *bitmap, int w, int h)
{
    chk_error_if(!bitmap, "bitmap was NULL.") return false;

    size_t required = chk_bitmap_query(w, h, bitmap->bpp);
    chk_error_if(bitmap->memory_size < required, "The bitmap would not fit into the current bitmap's buffer.") return false;

    bool resized = chk_bitmap_init(bitmap, w, h, bitmap->bpp, bitmap->memory, bitmap->memory_size);

    if (resized)
    {
        // Draw a dumb pattern
        uint8_t *row = bitmap->memory;
        for (int y = 0; y < bitmap->h; ++y)
        {
            uint32_t *pixel = (uint32_t *)row;
            for (int x = 0; x < bitmap->w; ++x)
            {
                *pixel++ = chk_packed_rgba_from_u8(x, y, 0, 255);
            }
            row += bitmap->stride;
        }
    }

    return resized;
}
