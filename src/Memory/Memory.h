#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "CartridgeHeader.h"
#include "MemoryDefs.h"
#include "MemoryInterface.h"

#include "Customizers/IOMemory.h"

const int ADDRESSABLE_MEMORY_SIZE = 0x10000;

// number of elements in the AddressRange enum
const int ADDRESS_RANGE_SIZE = 14;


/**
 * @file Memory.h
 * @brief Defines a thin layer on the top of all memory.
 *
 * This class assumes that a memory is tied to a particular cartridge
 * and that its functionality is implemented differently for different
 * ranges of addresses. However the functionality of any such ranges is
 * not decided here. Rather it is decided by the various implementations 
 * of MemoryCustomizer. This means that the only large data stored here
 * (actually, allocated by the loader, but it is managed here)
 * is the cartridge data. Each handler which registers for an address range
 * keeps track of the memory needed for that address range.
 */
class Memory : public ::MemoryInterface {

public:

    // let the user get header info easily
    CartridgeHeader* header;
    // points to the cartridge data
    data_t* cart;
    // length of cartridge data in bytes
    size_t cartSize;

    // takes the cartridge data and its size
    Memory( data_t* c, size_t cSize );
    ~Memory();

    // read from addr
    virtual data_t read( addr_t addr );
    // write val to addr
    virtual void write( addr_t addr, data_t val );

    enum AddressRange {
        // 0x0000-0x1FFF
        RomBank0_1 = 0,
        // 0x2000-0x3FFF
        RomBank0_2,
        // 0x4000-0x5FFF
        RomBanks_1,
        // 0x6000-0x7FFF
        RomBanks_2,
        // 0x8000-0x9FFF
        VRAM,
        // 0xA000-0xBFFF
        ERam,
        // 0xC000-0xCFFF
        WRam0,
        // 0xD000-0xDFFF
        WRam1,
        // 0xE000-0xFDFF
        ECHORAM,
        // 0xFE00-0xFE9F
        Oam,
        // 0xFEA0-0xFEFF
        NonUseable,
        // 0xFF00-0xFF7F
        IOPorts,
        // 0xFF80-0xFFFE
        HRam,
        // 0xFFFF
        IReg
    };

    /**
     * @brief Change the behavior of memory
     *
     * By registering listeners you can override the default behavior of 
     * @c read and @c write for specific memory segments.
     * This layer of memory guarantees that calls to registered
     * interfaces will be with addresses from their given range.
     * By calling this function mem is registered as both a read
     * and a write listener.
     * 
     * @param range The range of addresses for which @c mem should be used
     * @param mem The behavior intended for @c range
     */
    void registerListener( AddressRange range, MemoryInterface* mem );
    /**
     * Allow listening for just reads or just writes.
     */
    void registerReadListener( AddressRange range, MemoryInterface* mem );
    void registerWriteListener( AddressRange range, MemoryInterface* mem );
   
    /**
     * Gets the I/O Ports, which are required for a lot of the Game Boy components.
     */
    IOMemory *getIOMemory();
protected:

    // for now, I'll allocate enough space for all addresses to make things easy
    // TODO: don't
    data_t mem[ADDRESSABLE_MEMORY_SIZE];

    // the listeners for each address range
    MemoryInterface* readListeners[ADDRESS_RANGE_SIZE];
    MemoryInterface* writeListeners[ADDRESS_RANGE_SIZE];

    // I/O Ports
    IOMemory* ioMem;
};

#endif
