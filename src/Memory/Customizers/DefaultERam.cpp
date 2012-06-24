#include "DefaultERam.h"

DefaultERam::DefaultERam( Memory* mem ) {
    eram = new data_t[mem->header->romSize];
    eRamEnabled = false;
}

DefaultERam::~DefaultERam() {
    delete eram;
}

data_t DefaultERam::read( addr_t addr ) {
    return eram[addr-0xA000];
}

void DefaultERam::write( addr_t addr, data_t val ) {
    if( 0xA000 <= addr && addr <= 0xBFFF )
        eram[addr-0xA000] = val;
    else
        eRamEnabled = ( val == 0xA0 ) ? true : false;
}
