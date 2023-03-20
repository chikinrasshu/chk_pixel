#pragma once

#include "common.h"

typedef struct s_Bitmap
{
    int w, h, bpp;
    size_t stride;
    size_t size, memory_size;
    uint8_t *memory;

    bool owns_memory;
} Bitmap;

size_t chk_bitmap_query_stride(int w, int bpp);
size_t chk_bitmap_query(int w, int h, int bpp);

bool chk_bitmap_init(Bitmap *bitmap, int w, int h, int bpp, void *memory, size_t memory_size);
void chk_bitmap_destroy(Bitmap *bitmap);
bool chk_bitmap_resize(Bitmap *bitmap, int w, int h);
