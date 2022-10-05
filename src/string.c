#include "include/util.h"


#include <stdlib.h>

#include <string.h>

int endsWith(char* str, char* sub_str)
{
    int str_len = strlen(str);
    int sub_str_len = strlen(sub_str);

    if(sub_str_len > str_len)
    {
        return 0;
    }

    for (int i = 0; i < sub_str_len; i++)
    {
        if(str[(str_len - sub_str_len) + i] != sub_str[i])
        {
            return 0;
        }
    }

    return 1;
}

char* get_filename(char* file)
{
    char* filename = (char*) malloc(strlen(file) * sizeof(char) + 1);

    strcpy(filename, file);

    char* ext = strrchr(filename, '.');

    if(ext == NULL)
    {
        return 0;
    }

    *ext = '\0';

    return filename;
}

char* get_extension(char* file)
{
    char* filename = (char*) malloc(strlen(file) * sizeof(char) + 1);

    strcpy(filename, file);

    char* ext = strrchr(filename, '.');

    if(ext == NULL)
    {
        return 0;
    }

    return ext;
}

void sort_by_aplhabet(STRING_ARRAY* arr)
{
    for(int i = 0; i < arr->length; i++)
    {
        for(int j = i + 1; j < arr->length; j++)
        {
            if(strcmp(arr->data[i], arr->data[j]) > 0)
            {
                char* temp = (char*) malloc(strlen(arr->data[i]) * sizeof(char));

                strcpy(temp, arr->data[i]);
                strcpy(arr->data[i], arr->data[j]);
                strcpy(arr->data[j], temp);

                free(temp);
            }
        }
    }
}