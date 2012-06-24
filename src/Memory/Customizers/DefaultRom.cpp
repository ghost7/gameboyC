#include "DefaultRom.h"

DefaultRom::DefaultRom( data_t* c, size_t size ) : MemoryCustomizer( c, size ) {}

data_t DefaultRom::read( addr_t addr ) {
    return cart[addr];
}

void DefaultRom::write( addr_t addr, data_t val ) {}
