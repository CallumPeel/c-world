#include "fileHandler.h"

bool checkFileExists(const char* fileName)
{
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        return(false);
    }
    else
    {
        fclose(file);
        return(true);
    }
}

FILE* openFile(const char* fileName)
{
    bool exists = checkFileExists(fileName);
    if (!exists)
    {
        printf("File Not Found.\n");
    }
    return fopen(fileName, "r");
}
