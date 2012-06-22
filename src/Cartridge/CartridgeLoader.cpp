#include <stdlib.h>

#include "../Common/FileUtils.h"
#include "CartridgeLoader.h"
#include "CartridgeHeader.h"
#include "RomCartridge.h"
#include "RomType.h"

CartridgeBase *CartridgeLoader::loadCartridge(const char *fileName)
{
    uint8_t *rawRom = readFileToBuffer(fileName);
    if (rawRom == NULL)
    {
        return NULL;
    }
    
    CartridgeHeader header(rawRom);
    switch (header.romType)
    {
        case ROM:
            return new RomCartridge(rawRom);
        default :
            return NULL;
    }
}
