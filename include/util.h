#include <stdint.h>
#include <time.h>

int custom_sleep(uint32_t /* miliseconds */);

typedef struct STRING_ARRAY_STRUCT
{
    char** data;
    uint32_t length;
} STRING_ARRAY;

int endsWith(char*, char*);

char* get_filename(char*);

char* get_extension(char*);

void sort_by_aplhabet(STRING_ARRAY*);

int isDir(char*);

STRING_ARRAY* search_txt(const char*);

char* read_txt(const char*);