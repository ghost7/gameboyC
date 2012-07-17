#ifndef _LCD_BACKGROUND_H_
#define _LCD_BACKGROUND_H_

#include "../Memory/Customizers/IOMemory.h"
#include "../Memory/Memory.h"
#include "LcdComponent.h"

/**
 * Background component of the LCD.
 */
class LcdBackground : public LcdComponent
{
public:
    /**
     * Creates the LCD background.
     *
     * @param mem Memory to read VRAM data.
     * @param pixels Pixels to draw on.
     */
    LcdBackground(Memory* mem, uint32_t* pixels);

    virtual void drawScanline();

private:
    /**
     * Helper function to get the tile map address of the tiles that are 
     * currently being drawn as part of the scanline.
     */
    addr_t getTileMapAddress();

    /**
     * Gets the address of the actual tile that is indicated be a tile map.
     * 
     * @param tileMapAddress The address of the tile map.
     */
    addr_t getTileDataAddress(addr_t tileMapAddress);
};

#endif
