#include "Lcd.h"

#define LCD_ENABLED 0x80

#define LCD_STAT_REQUEST 0x2
#define LCD_STAT_ENABLED 0x40

#define VBLANK_REQUEST 0x1
#define VBLANK_ENABLED 0x10

Lcd::Lcd(Memory* mem)
{
    memory = mem;    
    ioPorts = memory->getIOMemory();
}

void Lcd::init(uint32_t* pixels)
{
    lcdPixels = pixels;
    background = new LcdBackground(memory, pixels);
}

void Lcd::step(int cycles)
{
    if (ioPorts->LCDC & LCD_ENABLED)
    {
        lcdCycles += cycles;
        switch (currentMode)
        {
            case HBlank:
                advanceHBlank();
                break;
            case VBlank:
                advanceVBlank();
                break;
            case SearchOAM:
                advanceSearchOam();
                break;
            case Transfer:
                advanceTransfer();
                break;
        }
    }
}

bool Lcd::isDirty()
{
    return dirty;
}

void Lcd::clean()
{
    dirty = false;
}

void Lcd::advanceHBlank()
{
    if (lcdCycles >= 204)
    {
        if (ioPorts->LY >= 144)
        {
            // The LCD could be marked dirty after every scanline has been drawn,
            // but it seems to make SDL lag if its set to be dirty that often. So
            // it is set to dirty after every scanline has been drawn, and the LCD
            // enters V-Blank.
            dirty = true;
            setMode(VBlank);
            if (ioPorts->STAT & VBLANK_ENABLED)
            {
                ioPorts->IFLAGS |= VBLANK_REQUEST;
            }
        }
        else
        {
            setMode(SearchOAM);
        }
    }
}

void Lcd::advanceVBlank()
{
    if (lcdCycles >= 465)
    {
        lcdCycles = 0;
        incrementScanLine();
        if (ioPorts->LY > 153)
        {
            ioPorts->LY = 0;
            setMode(SearchOAM);
        }
    }
}

void Lcd::advanceSearchOam()
{
    if (lcdCycles >= 80)
    {
        setMode(Transfer);
    }
}

void Lcd::advanceTransfer()
{
    if (lcdCycles >= 172)
    {
        setMode(HBlank);
        
        drawScanLine();
        incrementScanLine();
    }
}

void Lcd::setMode(LcdMode mode)
{
    currentMode = mode;
    ioPorts->STAT &= (0xFC | mode);
    lcdCycles = 0;
}

void Lcd::incrementScanLine()
{
    ioPorts->LY++;
    // Compare LY to LYC and request interrupt if:
    // 1. LY equals LYC
    // 2. STAT registers shows that the interrupt is enabled
    if ((ioPorts->LY == ioPorts->LYC) && (ioPorts->STAT & LCD_STAT_ENABLED))
    {
        ioPorts->IFLAGS |= LCD_STAT_REQUEST;
    }
}

void Lcd::drawScanLine()
{
    background->drawScanline();
}
