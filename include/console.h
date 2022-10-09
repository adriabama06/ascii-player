#pragma once

#include "include/util.h"

#include <stdint.h>

typedef struct thread_print_txt_args_s
{
    STRING_ARRAY* txt_files;
    char* base_input;
    int* base_input_len;
    uint32_t* print_i;
    uint8_t* end;
} THREAD_PRINT_TXT_ARGS;

void clear_console();

void* thread_print_txt(void* /* THREAD_PRINT_TXT_ARGS */);