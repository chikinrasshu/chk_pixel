#include "file_io.h"

#include <stdio.h>

size_t chk_get_file_size(const char *path)
{
    FILE *fptr = fopen(path, "rb+");
    chk_errorf_if(!fptr, "Failed to open file '%s'.", path) return 0;

    fseek(fptr, 0L, SEEK_END);
    size_t file_size = (size_t)ftell(fptr);
    fclose(fptr);
    return file_size;
}

size_t chk_read_binary_file(const char *path, void *memory, size_t memory_size)
{
    FILE *fptr = fopen(path, "rb+");
    chk_errorf_if(!fptr, "Failed to open file '%s'.", path) return 0;

    fseek(fptr, 0L, SEEK_END);
    size_t file_size = ftell(fptr);

    chk_errorf_if(file_size > memory_size,
                  "Not enough memory to read file '%s'. %zu bytes too short.", path, file_size - memory_size)
    {
        fclose(fptr);
        return file_size;
    }

    fseek(fptr, 0L, SEEK_SET);
    fread(memory, 1, file_size, fptr);
    fclose(fptr);

    return file_size;
}