#ifndef _GB_LCD_COMPONENT_H_
#define _GB_LCD_COMPONENT_H_

#include "../Common/Color.h"
#include "../Memory/Memory.h"
#include "../Memory/Customizers/IOMemory.h"

/**
 * Defines a component of the LCD.
 */
class LcdComponent
{
public:
    LcdComponent(Memory* mem, uint32_t* pixels);
    
    /**
     * Draws the current scanline, e.g LY
     */
    virtual void drawScanline() = 0;

    /**
     * Sets a pixel to be a certain color.
     *
     * @param pixels The array of pixels
     * @param x The x coordinate to set.
     * @param y The y coordinate to set.
     * @param Color The color to set.
     */
    static void setPixel(uint32_t* pixels, int x, int y, Color color);

    static Color getPixel(uint32_t* pixels, int x, int y);

    /**
     * Get a color based on a color code
     *
     * @param colorCode A color code that can be 0 to 4.
     */
    static Color getColor(int colorCode);

 protected:
    Memory* memory;
    IOMemory* ioPorts;
    uint32_t* lcdPixels;
};

#endif
