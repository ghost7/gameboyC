#include "mbc1Memory.h"

Mbc1Memory::Mbc1Memory( data_t* c, int cSize ) : Memory( c, cSize ) {
    romBank = 1;
    ramBank = 0;

    // If there's enough external RAM that it needs banks (32KB),
    // then allocate new memory for the ram pointer.
    // Otherwise, it can just point to the already allocated memory.
    if( ramSize > 8*KB_SIZE )
        ram = (data_t*) malloc( ramSize );
    else
        ram = mem + 0xA000;
}

Mbc1Memory::~Mbc1Memory() {
    if( ramSize > 8*KB_SIZE )
        delete ram;
}


/**
 * If MBC1 does something different with the address, then do that.
 * Otherwise, call the parent read function.
 *
 * @param addr The address whose value is requested
 * @return The value at {@code addr}
 */
data_t Mbc1Memory::read( addr_t addr ) {
    // TODO: make efficient, for now I'll keep it simple
    // ROM
    if( 0x4000 <= addr && addr <= 0x7FFF ) {
        return cart[ romBank*0x4000 + (addr-0x4000) ];
    }
    // external RAM
    else if( 0xA000 <= addr && addr <= 0xBFFF ) {
        // Even if external RAM is disabled, I'll let it do 
        // the same thing since the behavior is undefined.

        // Note that this won't segfault because 8KB is allocated 
        // in all cases, although that could change in the future.
        // Therefore, I can assume addr is valid and the worst that 
        // can happen is that I return an unexpected value; however,
        // that could only happen if they weren't supposed to be 
        // reading from external RAM in the first place.

        return ram[ ramBank*0x2000 + (addr-0xA000) ];
    }
    // default to the parent read
    return Memory::read( addr );
}


/**
 * If MBC1 does anything different for write at @code addr then do that.
 * Otherwise, call the parent write function.
 *
 * @param addr The address where {@code val} should be written
 * @param val The value to write
 */
void Mbc1Memory::write( addr_t addr, data_t val ) {
    // TODO: make efficient, for now keep it simple
    // select ROM bank (lower 5 bits)
    if( 0x2000 <= addr && addr <= 0x3FFF ) {
        data_t nval = (val == 0x00) ? 0x01 : val;
        romBank = (romBank & 0xE0) | (nval & 0x1F);
    }
    // select ROM bank (upper 2 bits)
    // OR
    // select RAM bank
    else if( 0x4000 <= addr && addr <= 0x5FFF ) {
        if( bankMode == ROM_BANKING_MODE )
            romBank = ((val & 0x03) << 5) | (romBank & 0x1F);
        else if( bankMode == RAM_BANKING_MODE )
            ramBank = val & 0x03;
    }
    // select banking mode
    else if( 0x6000 <= addr && addr <= 0x7FFF ) {
        if( (val & 0x01) == 1 )
            bankMode = RAM_BANKING_MODE;
        else
            bankMode = ROM_BANKING_MODE;
    }
    // external RAM
    else if( 0xA000 <= addr && addr <= 0xBFFF && eRamEnabled )
        ram[ ramBank*0x2000 + (addr-0xA000) ] = val;
    // default to parent write
    else 
        Memory::write( addr, val );
}
