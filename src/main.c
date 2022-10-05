#include "include/console.h"
#include "include/util.h"
#include "include/args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <pthread.h>
    #include <unistd.h>
#endif


void* preload(void* __data);

int main(int argc, const char* argv[])
{
    if(argc < 2)
    {
        printf("%s --help\n", argv[0]);
        return 0;
    }

    PROGRAM_USER_INPUT options = parseArguments(argc, argv);

    uint32_t ms_frame_delay = 1000 / options.fps;

    STRING_ARRAY* txt_files = search_txt(options.input_path);

    if(options.preload >= 1)
    {
        STRING_ARRAY* preload = (STRING_ARRAY*) malloc(sizeof(STRING_ARRAY));
        int preload_count = 0;

    }

    for (uint32_t i = 0; i < txt_files->length; i++)
    {
        clock_t time_corrector;
        time_corrector = clock();

        char* file = txt_files->data[i];

        char* file_input = (char*) malloc((options.input + strlen(file) + 2) * sizeof(char));

        strcpy(file_input, options.input_path);

        #ifdef _WIN32
            if (endsWith(options.input_path, "\\") == 0)
            {
                strcat(file_input, "\\");
            }
        #else
            if (endsWith(options.input_path, "/") == 0)
            {
                strcat(file_input, "/");
            }
        #endif

        strcat(file_input, file);

        char* image_data = read_txt(file_input);

        #ifdef _WIN32
            printf("\n");
        #else
            clear_console();
        #endif

        printf("%s\n", image_data);
        
        time_corrector = clock() - time_corrector;

        uint32_t time_removed_by_calcs = (time_corrector / CLOCKS_PER_SEC);

        if(ms_frame_delay < time_removed_by_calcs)
        {
            custom_sleep(ms_frame_delay);

        }
        else
        {
            custom_sleep(ms_frame_delay - time_removed_by_calcs);
        }
        free(file_input);
        free(image_data);
    }
    
    return 0;
}