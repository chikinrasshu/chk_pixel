#pragma once

#include "common.h"

size_t chk_get_file_size(const char *path);
size_t chk_read_binary_file(const char *path, void *memory, size_t memory_size);
