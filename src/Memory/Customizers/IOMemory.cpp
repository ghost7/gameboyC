#include "IOMemory.h"

IOMemory::IOMemory() {
    // size of IO ports
    for( int i = 0; i < 0x7F; i++ )
        mem[i] = 0;
}

data_t IOMemory::read( addr_t addr ) {
    return mem[addr-0xFF00];
}

void IOMemory::write( addr_t addr, data_t val ) {
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
