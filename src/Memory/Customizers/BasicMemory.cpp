#include <iostream>

#include "BasicMemory.h"

BasicMemory::BasicMemory( addr_t start, addr_t end ) {
    if( end <= start )
        // TODO: good errors
        std::cout << "Invalid initializers for BasicMemory." << std::endl;
    size = end - start + 1;
    offset = start;
    mem = new data_t[size];

    // set to 0 for consistency
    for( size_t i = 0; i < size; i++ )
        mem[i] = 0;
}

BasicMemory::~BasicMemory() {
    delete mem;
}

data_t BasicMemory::read( addr_t addr ) {
    return mem[addr-offset];
}

void BasicMemory::write( addr_t addr, data_t val ) {
    mem[addr-offset] = val;
}
