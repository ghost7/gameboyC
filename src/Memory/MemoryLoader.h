#ifndef _CARTIDGE_LOADER_H_
#define _CARTIDGE_LOADER_H_

#include <string>

#include "Memory.h"

/**
 * @brief Utility class for loading a cartridge from a file.
 *
 * @ingroup memory
 */
class MemoryLoader
{
public:
    /**
     * Loads a cartridge from a file and returns a Memory object
     *
     * @param fileName name of the file.
     * @return A new Memory object
     */
    static Memory* loadCartridge(const std::string fileName);
};

#endif
