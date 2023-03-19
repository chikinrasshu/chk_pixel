#include "cmd_list.h"

CmdList *chk_cmd_list_create(size_t size)
{
    if (!size)
    {
        fprintf(stderr, "chk_cmd_list_create: Size was 0.\n");
        return NULL;
    }

    CmdList *cmd_list = chk_alloc(sizeof(*cmd_list));
    if (!cmd_list)
    {
        fprintf(stderr, "chk_cmd_list_create: Failed to allocate the memory arena object.\n");
        return NULL;
    }

    if (!chk_cmd_list_init(cmd_list, size))
    {
        chk_free(cmd_list);
        return NULL;
    }

    return cmd_list;
}

void chk_cmd_list_free(CmdList *cmd_list)
{
    chk_cmd_list_destroy(cmd_list);
    chk_free(cmd_list);
}

bool chk_cmd_list_init(CmdList *cmd_list, size_t size)
{
    if (!chk_memory_arena_init(&cmd_list->arena, size))
        return false;

    cmd_list->count = 0;
    return true;
}

void chk_cmd_list_destroy(CmdList *cmd_list)
{
    chk_memory_arena_destroy(&cmd_list->arena);
    cmd_list->count = 0;
}
