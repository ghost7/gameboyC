#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string>
#include <stdio.h>

#include "FileUtils.h"

using namespace std;

char* readFileToBuffer(const std::string fileName, size_t* length)
{
    fstream file;
    file.open( fileName.c_str() , ios::in|ios::binary|ios::ate );
    if( !file.is_open() ) {
        // TODO: nice errors -- detect file existence
        std::cout << "couldn't open the file" << std::endl;
        return NULL;
    }

    *length = file.tellg();

    char* buffer = new char[*length];

    file.seekg( 0, ios::beg );
    file.read( buffer, (int)*length );
    
    file.close();
    return buffer;
}

size_t getFileLen(const std::string fileName)
{
    bool success;
    // Why does there have to be a '_' in windows?
    // Is there a better way to do this?
#ifdef _WIN32
    struct _stat64 buf;
    success = _stat64(fileName.c_str(), &buf) == 0;
#else
    struct stat64 buf;
    success = stat64(fileName.c_str(), &buf) == 0;  
#endif
    if (success)
    {
        return buf.st_size;
    }
    return 0;
}
