typedef struct PROGRAM_USER_INPUT_STRUCT
{    
    int input;
    char* input_path;

    float fps;
} PROGRAM_USER_INPUT;

void invalidArguments(int, int);

PROGRAM_USER_INPUT parseArguments(int argc, const char *argv[]);