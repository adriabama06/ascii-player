#include "include/util.h"

#include <stdio.h>
#include <stdlib.h>

char* read_txt(const char* input_file)
{
    FILE* image_text = fopen(input_file, "r");

    if(image_text == NULL)
    {
        printf("Error opening file: %s\n", input_file);
        exit(1);
    }

    long image_text_size = 0;

    fseek(image_text, 0, SEEK_END);

    image_text_size = ftell(image_text);

    if(image_text_size <= 0)
    {
        printf("Error reading file: %s\n", input_file);
        exit(1);
    }

    // fseek(fp, 0L, SEEK_SET); // go to the last position before last seek

    rewind(image_text); // go to the start of the file

    char* image_text_data = (char*) malloc(image_text_size * sizeof(char));

    fread(image_text_data, sizeof(char), image_text_size, image_text);

    fclose(image_text);

    image_text_data[image_text_size] = '\0';

    return image_text_data;
}