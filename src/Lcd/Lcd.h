#ifndef _LCD_H_
#define _LCD_H_

#include "LcdInterface.h"
#include "LcdBackground.h"
#include "../Memory/Memory.h"
#include "../Memory/Customizers/IOMemory.h"

/**
 * LCD implementation.
 */
class Lcd : public LcdInterface
{
public:
    /**
     * Creates a Game Boy LCD.
     *
     * @param mem Memory that the LCD can interface to.
     */
    Lcd(Memory* mem);

    virtual void init(uint32_t* pixels);
    virtual void step(int cycles);
    virtual bool isDirty();
    virtual void clean();
    
    /**
     * The Game Boy LCD goes through 4 different modes while
     * running.
     */
    enum LcdMode
    {
        HBlank = 0,
        VBlank = 1,
        SearchOAM = 2,
        Transfer = 3
    };
private:
    Memory* memory;
    IOMemory* ioPorts;
    uint32_t* lcdPixels;
    
    LcdBackground* background;

    bool dirty;
    int lcdCycles;
    LcdMode currentMode;

    /**
     * Advance the LCD when when in HBlank.
     */
    void advanceHBlank();
    
    /**
     * Advance the LCD when when in VBlank.
     */
    void advanceVBlank();
     
    /**
     * Advance the LCD when when searching OAM memory
     */
    void advanceSearchOam();
     
    /**
     * Advance the LCD when when transfering to the LCD driver.
     */
    void advanceTransfer();

    void setMode(LcdMode mode);
    void incrementScanLine();
    void drawScanLine();
};

#endif
