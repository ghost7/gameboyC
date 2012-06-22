#ifndef _FILE_UTIL_H_
#define _FILE_UTIL_H_

#include <stdint.h>

/**
 * Reads the contents of a file.
 *
 * \param fileName name of the file.
 * \param buffer pointer to store the contents of the file. Note this function
 *        will allocate memory for this buffer.
 * \return true if the file was successfully read in, false otherwise.
 */
uint8_t *readFileToBuffer(const char *fileName);

/**
 * Gets the length of a file
 *
 * \param fileName The name of the file.
 */
size_t getFileLen(const char *fileName);

#endif
