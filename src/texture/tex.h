#pragma once

#include <common.h>

#define TEX_MANAGER_BUCKET_COUNT 128

typedef struct s_TexKeyVal
{
    const char *key;
    void *value;
} TexKeyVal;

typedef struct s_TexManager
{
    TexKeyVal buckets[TEX_MANAGER_BUCKET_COUNT];
} TexManager;
