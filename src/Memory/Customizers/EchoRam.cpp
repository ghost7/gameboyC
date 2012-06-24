#include "EchoRam.h"

EchoRam::EchoRam( Memory* m ) {
    mem = m;
}

data_t EchoRam::read( addr_t addr ) {
    return mem->read( addr-0x2000 );
}

void EchoRam::write( addr_t addr, data_t val ) {}
