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

    uint32_t i = 0;
    uint8_t end = 0;

    THREAD_PRINT_TXT_ARGS args;

    args.txt_files = txt_files;
    args.base_input = options.input_path;
    args.base_input_len = &options.input;
    args.print_i = &i;
    args.end = &end;

    #ifdef _WIN32
        #define calc_win_time_to_ms(t) (t.wHour * 60 * 60 * 1000) + (t.wMinute * 60 * 1000) + (t.wSecond * 1000) + t.wMilliseconds

        HANDLE handle;

        handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) thread_print_txt, &args, 0, NULL);

        SYSTEMTIME start;
        GetSystemTime(&start);
        uint32_t start_pre_calc = calc_win_time_to_ms(start);

        SYSTEMTIME current;
    #else
        pthread_t handle;

        pthread_create(&handle, NULL, thread_print_txt, &args);

        clock_t start = clock();
    #endif

    while (i < txt_files->length)
    {
        #ifdef _WIN32
            GetSystemTime(&current);

            i = ((calc_win_time_to_ms(current)) - start_pre_calc) / ms_frame_delay;
        #else
            i = (clock() - start) / ms_frame_delay;
        #endif
    }
    
    end = 1;

    #ifdef _WIN32
        WaitForSingleObject(handle, INFINITE);
    #else
        pthread_join(handle, NULL);
    #endif

    return 0;
}