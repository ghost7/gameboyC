#ifndef _CARTIDGE_BASE_H_
#define _CARTIDGE_BASE_H_

#include <stdint.h>

/*! \defgroup cartridge Cartridge
 * Defines all logic to interact and load cartridges.
 */

/**
 * \brief Abstract Game Boy cartridge.
 *
 * \ingroup cartridge
 */
class CartridgeBase
{
public :
    /**
     * Read memory from the cartridge.
     *
     * \param memAddr memory address to read from.
     */
    virtual uint8_t read(uint16_t memAddr) = 0;
    
    /**
     * Write data to a cartridge.
     *
     * \param memAddr Memory address to write to.
     * \param data Data to write.
     */
    virtual void write(uint16_t memAddr, uint8_t data) = 0;
    
    /**
     * Creates a cartridge with the specified contents.
     *
     * \param rawRom Contents of the rom.
     */
    CartridgeBase(uint8_t *rawRom) { contents = rawRom; }
protected :
    /**
     * Contents of the rom.
     */
    uint8_t *contents;
};

#endif
