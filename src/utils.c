#include <stdio.h>
#include <stdlib.h>

void HandleError(const char* errorMessage)
{
    fprintf(stderr, "\033[0;31m");
    fprintf(stderr, "Error: %s\n", errorMessage);
    int c = getchar();
    exit(EXIT_FAILURE);
}