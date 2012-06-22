#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "memoryInterface.h"

const int ADDRESSABLE_MEMORY_SIZE = 0x10000;
MemoryInterface* loadCartridge( string cartName );


/**
 * @file memory.h
 *
 * This is the header for the class which defines
 * implementations of {@code MemoryInterface}'s {@code read} and {@code write}.
 * A memory cartridge in which there is no MBC is implemented here. 
 * MBC functionality should be implemented by extending this class.
 */
class Memory : public ::MemoryInterface {

public:

    // takes the cartridge data and its size
    Memory( data_t* c, int cSize );
    ~Memory();

    // read from addr
    virtual data_t read( addr_t addr );
    // write val to addr
    virtual void write( addr_t addr, data_t val );


protected:

    // for now, I'll allocate enough space for all addresses to make things easy
    data_t mem[ADDRESSABLE_MEMORY_SIZE];
    // points to the cartridge data
    data_t* cart;
    // length of cartridge data in bytes
    int cartSize;
    // length of CARTRIDGE! RAM in bytes
    int ramSize;
    // length of ROM in bytes
    int romSize;
    // is external RAM enabled?
    bool eRamEnabled;

    // header variable at 0x0148
    int getRomSize();
    // header variable at 0x0149
    int getRamSize();

};

#endif
