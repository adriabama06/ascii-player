#include "include/args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/util.h"

void invalidArguments(int i, int argc)
{
    if(i + 1 > argc)
    {
        printf("Invalid arguments, check if arguments are ok\n");
        exit(0);
    }
    return;
}

PROGRAM_USER_INPUT parseArguments(int argc, const char *argv[])
{
    PROGRAM_USER_INPUT options;

    options.input = 0;
    options.fps = 0.0f;

    for (int i = 1; i < argc; i++)
    {
        const char* arg = argv[i];
        
        if(strcmp(arg, "--help") == 0)
        {
            printf("%s -i <input folder> -fps <frame rate>\n", argv[0]);
            printf("  (Optional) --help - Show this help menu\n");
            printf("  -i - Input folder\n");
            printf("  -fps - Framerate in fps like: 24, 30, 60 or 23.976024, etc...\n");

            exit(0);
        }

        if(strcmp(arg, "-i") == 0)
        {
            invalidArguments(i, argc);

            i++;
            const char* value = argv[i];

            options.input = strlen(value);

            options.input_path = (char*) malloc(options.input * sizeof(char) + 1);

            strcpy(options.input_path, value);

            continue;
        }

        if(strcmp(arg, "-fps") == 0)
        {
            invalidArguments(i, argc);

            i++;
            const char* value = argv[i];

            options.fps = atof(value);

            continue;
        }

        if(strcmp(arg, "-p") == 0)
        {
            invalidArguments(i, argc);

            i++;
            const char* value = argv[i];

            options.preload = atoi(value);

            continue;
        }
    }

    if(options.input == 0)
    {
        printf("Please wrtie a input\n");

        exit(0);
    }

    if(isDir(options.input_path) == 0)
    {
        printf("The input need to be a folder\n");
    
        exit(0);
    }

    if(options.fps <= 0)
    {
        printf("The fps can't be 0 or negative\n");
    
        exit(0);
    }

    return options;
}