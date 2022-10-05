#include "include/console.h"
#include "include/util.h"
#include "include/args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif


typedef struct PRELOAD_ARGUMENTS_ARGS {
    STRING_ARRAY* txt_files;
    STRING_ARRAY* images_data;
    PROGRAM_USER_INPUT* options;
    uint32_t* images_ready;
    uint32_t* images_readed;
    int* wait_for_thread_launch;
} PRELOAD_ARGUMENTS;

void* preload(void* __data /* PRELOAD_ARGUMENTS */)
{
    PRELOAD_ARGUMENTS* data = (PRELOAD_ARGUMENTS*) __data;

    for (uint32_t i = 0; i < data->images_data->length;)
    {
        printf("Thread load: %d\n", i);
        if(*data->wait_for_thread_launch == 1 && i >= data->options->preload+1)
        {
            printf("%d\n", *data->wait_for_thread_launch);
            *data->wait_for_thread_launch = 0;
        }

        if((*data->images_ready) > (*data->images_readed + data->options->preload))
        {
            continue;
        }

        char* file = data->txt_files->data[i];

        char* file_input = (char*) malloc((data->options->input + strlen(file) + 2) * sizeof(char));

        strcpy(file_input, data->options->input_path);

        #ifdef _WIN32
            if (endsWith(data->options->input_path, "\\") == 0)
            {
                strcat(file_input, "\\");
            }
        #else
            if (endsWith(data->options->input_path, "/") == 0)
            {
                strcat(file_input, "/");
            }
        #endif

        strcat(file_input, file);

        char* image_data = read_txt(file_input);
        
        data->images_data->data[i] = image_data;

        *data->images_ready++;

        free(file_input);

        i++;
    }
    
    *data->images_ready = data->images_data->length + 1; // make sure the for at main thread will end

    return NULL;
}

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
        STRING_ARRAY* images_data = (STRING_ARRAY*) malloc(sizeof(STRING_ARRAY));

        images_data->data = (char**) malloc(txt_files->length * sizeof(char*));
        images_data->length = txt_files->length;
        uint32_t images_ready = 0;
        uint32_t images_readed = 0;

        int wait_for_thread_launch = 1;

        PRELOAD_ARGUMENTS* preload_args = (PRELOAD_ARGUMENTS*) malloc(sizeof(PRELOAD_ARGUMENTS));

        preload_args->txt_files = txt_files;
        preload_args->images_data = images_data;
        preload_args->options = &options;
        preload_args->images_ready = &images_ready;
        preload_args->images_readed = &images_readed;
        preload_args->wait_for_thread_launch = &wait_for_thread_launch;

        #ifdef _WIN32
            HANDLE preload_thread;

            preload_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) preload, preload_args, 0, NULL);
        #else
            pthread_t preload_thread;

            pthread_create(&preload_thread, NULL, preload, preload_args);
        #endif

        printf("Waiting for thread launch...\n");
        while (wait_for_thread_launch) {custom_sleep(ms_frame_delay * options.preload);}
        printf("Thread ready! Starting...");
        
        for (uint32_t i = 0; i < images_data->length;)
        {
            printf("main: %d\n", i);
            if(i >= images_ready) {
                custom_sleep(ms_frame_delay * options.preload);
                continue;
            }

            char* image_data = images_data->data[i];

            #ifdef _WIN32
                printf("\n");
            #else
                clear_console();
            #endif

            printf("%s\n", image_data);

            free(image_data);

            images_readed++;

            custom_sleep(ms_frame_delay);

            i++;
        }

        #ifdef _WIN32
            WaitForSingleObject(preload_thread, INFINITE);
        #else
            pthread_join(preload_thread, NULL);
        #endif
    }
    else
    {
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
            
            free(file_input);
            free(image_data);

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
        }
    }
    
    return 0;
}