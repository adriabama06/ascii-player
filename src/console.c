#include "include/console.h"

#include "include/util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#endif

void clear_console()
{
    printf("\e[1;1H\e[2J"); // this is more fast than system("clear") ! only for linux
}

void* thread_print_txt(void* __data)
{
    THREAD_PRINT_TXT_ARGS* data = (THREAD_PRINT_TXT_ARGS*) __data;

    size_t base_input_len = strlen(data->base_input);

    uint32_t last_i = 0;

    while (*data->end == 0)
    {
        if(last_i == *data->print_i)
        {
            continue;
        }

        char* file = data->txt_files->data[last_i];

        char* file_input = (char*) malloc((base_input_len + strlen(file) + 2) * sizeof(char));

        strcpy(file_input, data->base_input);

        #ifdef _WIN32
            if (endsWith(data->base_input, "\\") == 0)
            {
                strcat(file_input, "\\");
            }
        #else
            if (endsWith(data->base_input, "/") == 0)
            {
                strcat(file_input, "/");
            }
        #endif

        strcat(file_input, file);

        char* image_data = read_txt(file_input);

        #ifdef _WIN32
            putchar('\n');
        #else
            clear_console();
        #endif

        printf("%s", image_data);
        
        free(file_input);
        free(image_data);

        last_i = *data->print_i;
    }

    return NULL;
}

void hide_cursor()
{
    #ifdef _WIN32
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 10;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    #else
        printf("\e[?25l");
    #endif
    return;
}

void show_cursor()
{
    #ifdef _WIN32
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 10;
        info.bVisible = TRUE;
        SetConsoleCursorInfo(consoleHandle, &info);
    #else
        printf("\e[?25h");
    #endif
    return;
}
