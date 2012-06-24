#ifndef _FILE_UTIL_H_
#define _FILE_UTIL_H_

#include <stdint.h>
#include <string>

/**
 * Reads the contents of a file.
 * It's the user's responsibility to free the returned pointer.
 *
 * @param fileName Name of the file.
 * @param[out] length A pointer to which the length will be stored
 * @return A pointer to the file or NULL if it couldn't be read
 */
char* readFileToBuffer(const std::string fileName, size_t* length);

/**
 * Gets the length of a file
 *
 * @param fileName The name of the file.
 */
size_t getFileLen(const std::string fileName);

#endif
