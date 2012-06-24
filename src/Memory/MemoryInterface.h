#ifndef _MEMORY_INTERFACE_H_
#define _MEMORY_INTERFACE_H_

#include <cstring>

#include "MemoryDefs.h"
#include "MemoryInterface.h"

/**
 * @file memory.h
 *
 * This defines an interface for the entire game boy memory system.
 */
class MemoryInterface {

public:

    /**
     * Read according to the gameboy's memory map.
     * 
     * @param addr The address of memory requested
     * @return The value of memory (or whatever else the memory map specifies) at {@code addr}
     */
    virtual data_t read( addr_t addr) = 0;

    /**
     * Write data according to the gameboy's memory map.
     * 
     * @param addr The address to which the write should occur
     * @param val The value to write at {@code addr}
     */
    virtual void write( addr_t addr, data_t data) = 0;
};

#endif
