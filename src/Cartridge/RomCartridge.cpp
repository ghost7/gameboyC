#include "RomCartridge.h"

RomCartridge::RomCartridge(uint8_t *rawRom) : CartridgeBase(rawRom)
{
}

uint8_t RomCartridge::read(uint16_t memoryAddr)
{
    return contents[memoryAddr];
}

void RomCartridge::write(uint16_t memAddr, uint8_t data)
{

}
