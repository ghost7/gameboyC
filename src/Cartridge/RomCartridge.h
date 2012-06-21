#ifndef _ROM_CARTRIDGE_H_
#define _ROM_CARTRIDGE_H_

#include "CartridgeBase.h"

/**
 * \brief Game Boy cartridge that only supports read-only memory.
 *
 * \ingroup cartridge
 */
class RomCartridge : public CartridgeBase
{
public:
    /**
     * Creates a cartridge with the specified contents.
     *
     * \param rawRom Contents of the rom.
     */
    RomCartridge(uint8_t *rawRom);
    
    /**
     * Read memory from the cartridge.
     *
     * \param memAddr memory address to read from.
     */
    uint8_t read(uint16_t memAddr);
    
    /**
     * Doesn't do anything, as the cartridge is read-only.
     */
    void write(uint16_t memAddr, uint8_t data);
};

#endif
