#include "include/console.h"

#include <stdio.h>
#include <stdlib.h>

void clear_console()
{
    // #ifdef _WIN32
    //     system("cls");
    // #else
    //     system("clear");
    // #endif

    printf("\e[1;1H\e[2J"); // this is more fast than system()
}

