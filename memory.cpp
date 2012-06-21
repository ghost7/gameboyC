#include "memoryHeaders.h"

/** 
 * Create a new {@code Memory} object from the given gameboy cartridge file.
 * 
 * @param c A pointer to the cartridge data
 * @param cSize The length of the cartridge data
 */
Memory::Memory( data_t* c, int cSize ) {

    cart = c;
    cartSize = cSize;
    
    if( cartSize < 0x148 ) {
        cout << "The cartridge is apparently way to small.";
        throw;
    }
    romSize = getRomSize();
    ramSize = getRamSize();
    // game boy define default
    eRamEnabled = false;

    // set it all to 0 for consistency
    for( int i = 0; i < ADDRESSABLE_MEMORY_SIZE; i++ ) {
        mem[i] = 0;
    }
}


Memory::~Memory() {
    delete cart;
}


/**
 * Read from the given address according to the gameboy's memory map.
 *
 * @param addr The address whose value is requested
 * @return The value at {@code addr}
 */
data_t Memory::read( addr_t addr ) {
    // TODO: make efficient, for now I'll keep it simple
    // ROM
    if( addr <= 0x7FFF ) {
        return cart[addr];
    }
    // VRAM
    else if( 0x8000 <= addr && addr <= 0x9FFF ) {
        // curious... recursion with depth 1 (unless read(0xFF41) is changed)
        if( ((read(0xFF41)&0x03) == 3) && ((read(0xFF40)&0x80) == 0x80) )
            return 0xFF;
        return mem[addr];
    }
    // external RAM
    else if( 0xA000 <= addr && addr <= 0xBFFF ) {
        return mem[addr];
    }
    // internal RAM
    else if( 0xC000 <= addr && addr <= 0xDFFF ) {
        return mem[addr];
    }
    // echo of internal RAM
    else if( 0xE000 <= addr && addr <= 0xFDFF ) {
        return mem[addr-0x2000];
    }
    // OAM
    else if( 0xFE00 <= addr && addr <= 0xFE9F ) {
        if( ((read(0xFF41)&0x03) > 1) && ((read(0xFF40)&0x80) == 0x80) )
            return 0xFF;
        return mem[addr];
    }
    // unusable RAM is left unusable
    // IO ports
    else if( 0xFF00 <= addr && addr <= 0xFF7F ) {
        return mem[addr];
    }
    // HRAM
    else if( 0xFF80 <= addr && addr <= 0xFFFE ) {
        return mem[addr];
    }
    // Interrupt Enable
    else if( addr == 0xFFFF ) {
        return mem[addr];
    }
    return mem[addr];
}


/**
 * Write the given value to the given address in the gameboy's memory space.
 *
 * @param addr The address where {@code val} should be written
 * @param val The value to write
 */
void Memory::write( addr_t addr, data_t val ) {
    // TODO: make efficient, for now keep it simple
    // ROM
    if( addr <= 0x1FFF ) {
        eRamEnabled = ( val == 0xA0 ) ? true : false;
    }
    // VRAM
    else if( 0x8000 <= addr && addr <= 0x9FFF ) {
        mem[addr] = val;
    }
    // external RAM
    else if( 0xA000 <= addr && addr <= 0xBFFF ) {
        mem[addr] = val;
    }
    // internal RAM
    else if( 0xC000 <= addr && addr <= 0xDFFF ) {
        mem[addr] = val;
    }
    // echo of internal RAM
    /*else if( 0xE000 <= addr && addr <= 0xFDFF ) {
        return mem[addr-0x2000];
    }*/
    // OAM
    else if( 0xFE00 <= addr && addr <= 0xFE9F ) {
        mem[addr] = val;
    }
    // unusable RAM is left unusable
    // IO ports
    else if( 0xFF00 <= addr && addr <= 0xFF7F ) {
        // TODO: this isn't how it works :(
        if( addr == 0xFF44 )
            return;
        else if( addr == 0xFF04 )
            mem[addr] = 0;
        // write only 7-4
        else if( addr == 0xFF1C )
            mem[addr] = (val&0xF0) | (mem[addr]&0x0F);
        // write only 7-3
        else if( addr == 0xFF41 )
            mem[addr] = val; //(val&0xF8) | (mem[addr]&0x07);
        else
            mem[addr] = val;
    }
    // HRAM
    else if( 0xFF80 <= addr && addr <= 0xFFFE ) {
        mem[addr] = val;
    }
    // Interrupt Enable
    else if( addr == 0xFFFF ) {
        mem[addr] = val;
    }
}


/**
 * Translates the value in the cartridge at 0x0148
 * into the supposed ROM size in bytes.
 * 
 * @pre The cartridge (@c cart) must be initialized
 * @return Claimed ROM size in bytes or 32KB if not recognized
 */
int Memory::getRomSize() {
    data_t romType = cart[0x0148];
    switch( romType ) {
        case 0x00: return 32*KB_SIZE;
        case 0x01: return 64*KB_SIZE;
        case 0x02: return 128*KB_SIZE;
        case 0x03: return 256*KB_SIZE;
        case 0x04: return 512*KB_SIZE;
        case 0x05: return 1024*KB_SIZE;
        case 0x06: return 2048*KB_SIZE;
        /* Not supported 
        case 0x07: return 4096*KB_SIZE; */
        case 0x52: return 1152*KB_SIZE;
        case 0x53: return 1280*KB_SIZE;
        case 0x54: return 1536*KB_SIZE;
    }
    return 32*KB_SIZE;
}


/**
 * Translates the cart value at 0x0149 
 * to a supposed external RAM size.
 *
 * @pre The cartridge (@c cart) must be initialized
 * @return The claimed size of external RAM in bytes or 0 if not recognized
 */
int Memory::getRamSize() {
    data_t ramType = cart[0x0149];
    switch( ramType ) {
        case 0x00: return 0;
        case 0x01: return 2*KB_SIZE;
        case 0x02: return 8*KB_SIZE;
        case 0x03: return 32*KB_SIZE;
    }
    return 0;
}



/**
 * Loads the given cartridge into the appropriate memory object according 
 * to which type of cartridge is given.
 * The user is responsible for deleting the returned pointer!
 * 
 * @param cartName The name of the file which holds the cartridge
 * @return A new memory object or NULL if the load failed
 */
MemoryInterface* loadCartridge( string cartName ) {
    fstream cartFile;
    cartFile.open( cartName , 
        ios::out|ios::in|ios::binary|ios::ate );
    if( !cartFile.is_open() )
        return NULL;

    MemoryInterface* result = NULL;

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
