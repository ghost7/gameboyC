#include <stdio.h>
#include "LcdBackground.h"

#define TILE_MAP_SELECT 0x4
#define TILE_DATA_SELECT 0x10

LcdBackground::LcdBackground(Memory* mem, uint32_t* pixels) 
    : LcdComponent(mem, pixels)  
{
}

void LcdBackground::drawScanline()
{
    addr_t tileMapAddress = getTileMapAddress();
    addr_t tileDataAddress = getTileDataAddress(tileMapAddress);
    
    data_t tileLow = memory->read(tileDataAddress);
    data_t tileHigh = memory->read(tileDataAddress + 1);

    int tileX = ioPorts->SCX % 8;
    for (int x = 0; x < 160; x++)
    {
        // Draw the current tile.
        int paletteIndex = (tileLow >> (7 - tileX) & 1);
        paletteIndex |= (tileHigh >> (7 - tileX) & 1) << 1;
        int colorIndex = (ioPorts->BGP >> (paletteIndex * 2)) & 0x3;

        setPixel(lcdPixels, x, ioPorts->LY, getColor(colorIndex));

        tileX++;
        if (tileX == 8)
        {
            // The current tile has finished drawing, move on the next one.
            tileX = 0;
            tileMapAddress++;
            tileDataAddress = getTileDataAddress(tileMapAddress);
            tileLow = memory->read(tileDataAddress);
            tileHigh = memory->read(tileDataAddress + 1);
        }
    }
}

addr_t LcdBackground::getTileMapAddress()
{
    // base address of the tile map
    addr_t mapAddress = (ioPorts->LCDC & TILE_MAP_SELECT) == 0 ? 0x9800 : 0x9C00;
    
    // Add x and y offsets.
    mapAddress += ((ioPorts->SCY + ioPorts->LY) % 256) / 8 * 32;
    mapAddress += ioPorts->SCX / 8;
    return mapAddress;
}

addr_t LcdBackground::getTileDataAddress(addr_t tileMapAddress)
{
    // Tile number and base tile data address.
    data_t tileNum = memory->read(tileMapAddress);
    addr_t tileAddress = (ioPorts->LCDC & TILE_DATA_SELECT) == 0 ? 0x8800 : 0x8000;

    // Get the address of the tile. TODO take negative values into account.
    tileAddress += tileNum * 16;
    
    // Add the offset of the row that is being displayed.
    tileAddress += ((ioPorts->SCY + ioPorts->LY) % 8) * 2;
    return tileAddress;
}
