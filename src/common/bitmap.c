#include "bitmap.h"

#include <string.h>

// Creates a new bitmap, allocating just enough memory to fit it's internal buffers.
// This simple implementation also serves as documentation as to how to create a bitmap
// when not using complex memory allocators or memory arenas. (ex: just chk_alloc/chk_free)
Bitmap chk_bitmap_create(int w, int h, int bpp)
{
    Bitmap bmp = {0};
    bmp.memory_size = chk_bitmap_init(&bmp, w, h, bpp);
    bmp.memory = chk_alloc(bmp.memory_size);
    chk_bitmap_resize(&bmp, w, h);
    return bmp;
}

// If no bitmap is provided, it returns how much memory to allocate to fit the bitmap at the current w,h and bpp (bytesperpixel).
// If it's provided one, it's assumed it has enough storage, set's up the stride for the current bpp, and clears the memory block to zero.
size_t chk_bitmap_init(Bitmap *bmp, int w, int h, int bpp)
{
    size_t needed_stride = (((size_t)w * bpp + 3) / 4) * 4;
    size_t needed_memory = needed_stride * (size_t)h * sizeof(uint8_t);
    if (!bmp)
        return needed_memory;
    if (bmp->memory_size < needed_memory)
        return needed_memory;

    bmp->w = w;
    bmp->h = h;
    bmp->bpp = bpp;
    bmp->stride = needed_stride;
    bmp->size = needed_memory;

    // Zero the memory block we're interested in only.
    chk_zero_memory(bmp->memory, bmp->size);

    return bmp->size;
}

// Resets the bitmap to an invalid state. It does not deallocate the memory. Useful if the bitmap's memory is managed by someone else.
void chk_bitmap_destroy(Bitmap *bmp)
{
    bmp->w = 0;
    bmp->h = 0;
    bmp->size = 0;
    bmp->stride = 0;
    bmp->bpp = 0;
}

// Resets the bitmap to an invalid state, then it deallocates the memory. Useful if the bitmap was created with chk_bitmap_create(...)
void chk_bitmap_free(Bitmap *bmp)
{
    chk_bitmap_destroy(bmp);

    chk_free(bmp->memory);
    bmp->memory = NULL;
    bmp->memory_size = 0;
}

// If the bitmap's memory is too small, it returns the needed amount for resizing.
// If there is enough memory, then it simply calculates the stride for the new size.
size_t chk_bitmap_resize(Bitmap *bmp, int w, int h)
{
    if (bmp && bmp->w == w && bmp->h == h)
        return bmp->size;

    return chk_bitmap_init(bmp, w, h, bmp->bpp);
}

// Returns how much memory would be needed to store the bitmap with the requested dimensions
size_t chk_bitmap_query_size(int w, int h, int bpp)
{
    return chk_bitmap_init(NULL, w, h, bpp);
}