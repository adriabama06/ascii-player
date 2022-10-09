#include "include/console.h"

#include "include/util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

        #ifndef _WIN32
            clear_console();
        #endif

        printf("%s\n", image_data);
        
        free(file_input);
        free(image_data);

        last_i = *data->print_i;
    }

    return NULL;
}