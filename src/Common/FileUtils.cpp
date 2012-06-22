#include <stdio.h>
#include <sys/stat.h>

#include "FileUtils.h"

uint8_t *readFileToBuffer(const char *fileName)
{
    FILE *fp = fopen(fileName, "r");
    if (!fp)
        return NULL;
    size_t len = getFileLen(fileName);
    uint8_t *buffer = new uint8_t[len + 1];
    buffer[fread(buffer, 1, len, fp)] = 0;
    fclose(fp);
    return buffer;
}

size_t getFileLen(const char *fileName)
{
    struct stat64 buf;
    if (stat64(fileName, &buf) == 0)
    {
        return buf.st_size;
    }
    return 0;
}
