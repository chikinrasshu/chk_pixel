#pragma once

#include "common.h"

typedef struct s_Bitmap
{
    int w, h, bpp;
    size_t stride;
    size_t size, memory_size;
    uint8_t *memory;
} Bitmap;

Bitmap chk_bitmap_create(int w, int h, int bpp);
size_t chk_bitmap_init(Bitmap *bmp, int w, int h, int bpp);
void chk_bitmap_destroy(Bitmap *bmp);
void chk_bitmap_free(Bitmap *bmp);
size_t chk_bitmap_resize(Bitmap *bmp, int w, int h);
size_t chk_bitmap_query_size(int w, int h, int bpp);
