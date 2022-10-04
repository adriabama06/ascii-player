#include "include/util.h"

#include <time.h>

int custom_sleep(uint32_t ms /* milliseconds */)
{
    struct timespec ts;
    
    ts.tv_sec = ms / 1000; // seconds
    ts.tv_nsec = (ms % 1000) * 1000000; // nano seconds

    int res = nanosleep(&ts, &ts);
    
    return res;
}