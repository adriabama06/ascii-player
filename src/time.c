#include "include/util.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#include <time.h>

void custom_sleep(uint32_t ms /* milliseconds */)
{
    #ifdef _WIN32
        Sleep(ms);
    #else
        struct timespec ts;
    
        ts.tv_sec = ms / 1000; // seconds
        ts.tv_nsec = (ms % 1000) * 1000000; // nano seconds

        int res = nanosleep(&ts, &ts);
    #endif
    
    return;
}