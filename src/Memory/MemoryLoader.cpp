#include <stdlib.h>
#include <iostream>

#include "../Common/FileUtils.h"
#include "CartridgeHeader.h"
#include "MemoryLoader.h"
#include "Memory.h"

Memory* MemoryLoader::loadCartridge(const std::string fileName)
{
    size_t romSize;
    data_t* rawRom = (data_t *)readFileToBuffer(fileName, &romSize);
    if( rawRom == NULL ) {
        std::cout << "couldn't load the raw rom into memory!" << std::endl;
        // TODO: throw nice errors
        return NULL;
    }
    
    Memory* result = new Memory( rawRom, romSize );
    return result;
}



// TODO: remove eventually
/*
    // this is equal to the size when opened with ios::ate
    // I can use an int when it's under 2GB
    int cartSize = (int) cartFile.tellg();

    cartFile.seekg( 0, ios::beg );
    // copy the cartridge into malloc'd memory
    data_t* cart = (data_t*) malloc( cartSize * sizeof(data_t) );
    cartFile.read( (char*)cart, cartSize );
    // determine cartridge type
    cartFile.seekg( 0x0147, ios::beg );
    data_t cartType;
    cartFile.read( (char*)(&cartType), sizeof(data_t) );

    // No MBC
    if( cartType == 0x00 || cartType == 0x08 || cartType == 0x09 ) {
        result = new Memory( cart, cartSize );
    }
    // MBC1
    else if( 0x01 <= cartType && cartType <= 0x03 )
        result = new Mbc1Memory( cart, cartSize );
    // MBC2
    else if( cartType == 0x05 || cartType == 0x06 )
        result = new Mbc2Memory( cart, cartSize );
    // MBC3
    else if( 0x0F <= cartType && cartType <= 0x13 )
        result = new Mbc3Memory( cart, cartSize );
    // MBC5
    else if( 0x19 <= cartType && cartType <= 0x1E )
        result = new Mbc5Memory( cart, cartSize );
    // not supported
    else {
        cout << "Cartridge type not supported.";
        result = NULL;
        delete cart;
    }

    return result;
}
*/
