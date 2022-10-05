#include "include/console.h"

#include <stdio.h>

void clear_console()
{
    printf("\e[1;1H\e[2J"); // this is more fast than system() ! only for linux
}
