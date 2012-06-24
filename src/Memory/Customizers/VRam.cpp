#include "VRam.h"

VRam::VRam( Memory* m ) {
    memObj = m;
    
    size = 0x2000;
    mem = new data_t[size];

    // set to 0 for consistency
    for( size_t i = 0; i < size; i++ )
        mem[i] = 0;
}

VRam::~VRam() {
    delete mem;
}

data_t VRam::read( addr_t addr ) {
    // recursion with depth 1 in memObj
    if( ((memObj->read(0xFF41)&0x03) == 3) && ((memObj->read(0xFF40)&0x80) == 0x80) )
        return 0xFF;
    return mem[addr-0x8000];
}

void VRam::write( addr_t addr, data_t val ) {
    mem[addr-0x8000] = val;
}
