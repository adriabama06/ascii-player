#include "include/console.h"
#include "include/util.h"
#include "include/args.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <pthread.h>
    #include <sys/time.h>
    #include <time.h>
#endif

void intHandler(int dummy) {
    show_cursor();

    exit(0);
}

int main(int argc, const char* argv[])
{
    if(argc < 2)
    {
        printf("%s --help\n", argv[0]);
        return 0;
    }

    PROGRAM_USER_INPUT options = parseArguments(argc, argv);

    uint32_t frame_delay = 1000000 / options.fps;

    STRING_ARRAY* txt_files = search_txt(options.input_path);

    uint32_t i = 0;
    uint8_t end = 0;

    THREAD_PRINT_TXT_ARGS args;

    args.txt_files = txt_files;
    args.base_input = options.input_path;
    args.base_input_len = &options.input;
    args.print_i = &i;
    args.end = &end;

    signal(SIGINT, intHandler);

    hide_cursor();

    #ifdef _WIN32
        #define calc_win_time_to_ms(t) (t.wHour * 60 * 60 * 1000) + (t.wMinute * 60 * 1000) + (t.wSecond * 1000) + t.wMilliseconds

        HANDLE handle;

        handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) thread_print_txt, &args, 0, NULL);

        SYSTEMTIME start;
        GetSystemTime(&start);
        uint32_t start_pre_calc = calc_win_time_to_ms(start);

        SYSTEMTIME current;
    #else
        #define timeval_to_nanoseconds(t) ((t.tv_sec * 1000000) + t.tv_usec)

        pthread_t handle;

        pthread_create(&handle, NULL, thread_print_txt, &args);

        struct timeval start;
        gettimeofday(&start, NULL);
        #ifdef __UINT64_TYPE__
            uint64_t start_pre_calc = timeval_to_nanoseconds(start);
            #ifdef DEBUG_TYPE
                printf("Using uint64_t\n");
            #endif
        #else
            #ifdef DEBUG_TYPE
                printf("Not precalculating start time because the computer don't support uint64_t\n");
            #endif
        #endif

        struct timeval current;
    #endif

    #ifdef DEBUG_TIME
        uint32_t last_i = 0;
    #endif

    while (i < txt_files->length)
    {
        #ifdef _WIN32
            GetSystemTime(&current);

            i = (((calc_win_time_to_ms(current)) - start_pre_calc) * 1000) / frame_delay;
        #else
            gettimeofday(&current, NULL);
            
            #ifdef __UINT64_TYPE__
                i = (timeval_to_nanoseconds(current) - start_pre_calc) / frame_delay;
            #else
                i = (timeval_to_nanoseconds(current) - timeval_to_nanoseconds(start)) / frame_delay;
            #endif
        #endif

        #ifdef DEBUG_TIME
            if(i != last_i)
            {
                last_i = i;
                #ifdef _WIN32
                    printf("%d - %d = %d / %d => %d\n", calc_win_time_to_ms(current), start_pre_calc, (calc_win_time_to_ms(current) - start_pre_calc), frame_delay, i);
                #else
                    printf("%d - %d = %d / %d => %d\n", timeval_to_nanoseconds(current), timeval_to_nanoseconds(start), (timeval_to_nanoseconds(current) - timeval_to_nanoseconds(start)), frame_delay, i);
                #endif
            }
        #endif
    }
    
    end = 1;

    #ifdef _WIN32
        WaitForSingleObject(handle, INFINITE);
    #else
        pthread_join(handle, NULL);
    #endif

    show_cursor();

    return 0;
}