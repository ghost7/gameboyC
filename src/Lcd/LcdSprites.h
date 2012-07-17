#ifndef _LCD_SPRITES_H_
#define _LCD_SPRITES_H_

#include "../Memory/Customizers/IOMemory.h"
#include "../Memory/Memory.h"
#include "LcdComponent.h"

class LcdSprites : public LcdComponent
{
public:
    LcdSprites(Memory* mem, uint32_t* pixels);
    virtual void drawScanline();
};

#endif
