#ifndef _CARTIDGE_LOADER_H_
#define _CARTIDGE_LOADER_H_

#include "CartridgeBase.h"

/**
 * \brief Utility class for loading a cartridge from a file.
 *
 * \ingroup cartridge
 */
class CartridgeLoader
{
public:
    /**
     * Loads a cartridge from a file.
     *
     * \param fileName name of the file.
     * \return Cartridge that can be inserted into memory.
     */
    static CartridgeBase *loadCartridge(const char *fileName);
};

#endif
