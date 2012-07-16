#include <iostream>

#include "CartridgeHeader.h"
#include "Memory.h"
#include "Customizers/BasicMemory.h"
#include "Customizers/DefaultERam.h"
#include "Customizers/DefaultRom.h"
#include "Customizers/EchoRam.h"
#include "Customizers/IOMemory.h"
#include "Customizers/LazyMemory.h"
#include "Customizers/VRam.h"

/** 
 * Create a new {@code Memory} object from the given gameboy cartridge file.
 * 
 * @param c A pointer to the cartridge data
 * @param cSize The length of the cartridge data
 */
Memory::Memory( data_t* c, size_t cSize )  {

    cart = c;
    cartSize = cSize;
    
    if( cartSize < 0x148 ) {
        // TODO: decide about error handling (look for other cout uses)
        std::cout << "The cartridge is apparently way to small.";
        throw;
    }

    header = new CartridgeHeader( c, cSize );

    MemoryInterface* mem = new DefaultRom( cart, cartSize );
    registerReadListener( RomBank0_1, mem );
    registerListener( RomBank0_2, mem );
    registerListener( RomBanks_1, mem );
    registerListener( RomBanks_2, mem );
    
    mem = new DefaultERam( this );
    registerWriteListener( RomBank0_1, mem );
    registerListener( ERam, mem );

    // Register VRAM
    MemoryInterface* vRam = new VRam(this);
    registerListener(VRAM, vRam);

    // Register Work Ram Bank 0
    MemoryInterface* workRam0 = new BasicMemory(0xC000, 0xCFFF);
    registerListener(WRam0, workRam0);

    // Register Work Ram Bank 1
    MemoryInterface* workRam1 = new BasicMemory(0xD000, 0xDFFF);
    registerListener(WRam1, workRam1);

    // Register Echo Ram
    MemoryInterface* echoRam = new EchoRam(this);
    registerListener(ECHORAM, echoRam);

    // Register OAM Ram
    MemoryInterface* oamRam = new BasicMemory(0xFE00, 0xFE9F);
    registerListener(Oam, oamRam);

    // Register Non-Usable memory
    MemoryInterface* nonUsable = new LazyMemory();
    registerListener(NonUseable, nonUsable);

    // Register IOPorts
    ioMem = new IOMemory();
    registerListener(IOPorts, ioMem);
    
    // Register High-Speed ram.
    MemoryInterface* hRam = new BasicMemory(0xFF80, 0xFFFE);
    registerListener(HRam, hRam);

    // Regsiter Interrupt Enable 
    MemoryInterface* intEnable = new BasicMemory(0xFFFF, 0xFFFF);
    registerListener(IReg, intEnable);

    dmg = new DmgBoot();
}


Memory::~Memory() {
    delete cart;
    delete header;
}


/**
 * Read from the given address according to the gameboy's memory map.
 *
 * @param addr The address whose value is requested
 * @return The value at {@code addr}
 */
data_t Memory::read( addr_t addr ) 
{
    if (dmg->isEnabled() && addr <= 0xFF)
        return dmg->read(addr);
    // ROM
    else if( addr <= 0x1FFF )
        return readListeners[RomBank0_1]->read( addr );
    else if( 0x2000 <= addr && addr <= 0x3FFF )
        return readListeners[RomBank0_2]->read( addr );
    else if( 0x4000 <= addr && addr <= 0x5FFF )
        return readListeners[RomBanks_1]->read( addr );
    else if( 0x6000 <= addr && addr <= 0x7FFF )
        return readListeners[RomBanks_2]->read( addr );
    // VRAM
    else if( 0x8000 <= addr && addr <= 0x9FFF )
        return readListeners[VRAM]->read( addr );
    // external RAM
    else if( 0xA000 <= addr && addr <= 0xBFFF )
        return readListeners[ERam]->read( addr );
    // internal RAM part 1
    else if( 0xC000 <= addr && addr <= 0xCFFF )
        return readListeners[WRam0]->read( addr );
    // internal RAM part 2
    else if( 0xD000 <= addr && addr <= 0xDFFF )
        return readListeners[WRam1]->read( addr );
    // echo of internal RAM
    else if( 0xE000 <= addr && addr <= 0xFDFF )
        return readListeners[ECHORAM]->read( addr );
    // OAM
    else if( 0xFE00 <= addr && addr <= 0xFE9F )
        return readListeners[Oam]->read( addr );
    // unusable RAM is left unusable
    else if( 0xFEA0 <= addr && addr <= 0xFEFF )
        return readListeners[NonUseable]->read( addr );
    // IO ports
    else if( 0xFF00 <= addr && addr <= 0xFF7F )
        return readListeners[IOPorts]->read( addr );
    // HRAM
    else if( 0xFF80 <= addr && addr <= 0xFFFE )
        return readListeners[HRam]->read( addr );
    // Interrupt Enable
    else if( addr == 0xFFFF )
        return readListeners[IReg]->read( addr );
    return 0;
}


/**
 * Write the given value to the given address in the gameboy's memory space.
 *
 * @param addr The address where {@code val} should be written
 * @param val The value to write
 */
void Memory::write( addr_t addr, data_t val ) 
{
    if (dmg->isEnabled() && addr <= 0xFF)
        dmg->write(addr, val);
    // ROM
    else if( addr <= 0x1FFF )
        writeListeners[RomBank0_1]->write( addr, val );
    else if( 0x2000 <= addr && addr <= 0x3FFF )
        writeListeners[RomBank0_2]->write( addr, val );
    else if( 0x4000 <= addr && addr <= 0x5FFF )
        writeListeners[RomBanks_1]->write( addr, val );
    else if( 0x6000 <= addr && addr <= 0x7FFF )
        writeListeners[RomBanks_2]->write( addr, val );
    // VRAM
    else if( 0x8000 <= addr && addr <= 0x9FFF )
        writeListeners[VRAM]->write( addr, val );
    // external RAM
    else if( 0xA000 <= addr && addr <= 0xBFFF )
        writeListeners[ERam]->write( addr, val );
    // internal RAM part 1
    else if( 0xC000 <= addr && addr <= 0xCFFF )
        writeListeners[WRam0]->write( addr, val );
    // internal RAM part 2
    else if( 0xD000 <= addr && addr <= 0xDFFF )
        writeListeners[WRam1]->write( addr, val );
    // echo of internal RAM
    else if( 0xE000 <= addr && addr <= 0xFDFF )
        writeListeners[ECHORAM]->write( addr, val );
    // OAM
    else if( 0xFE00 <= addr && addr <= 0xFE9F )
        writeListeners[Oam]->write( addr, val );
    // unusable RAM is left unusable
    else if( 0xFEA0 <= addr && addr <= 0xFEFF )
        writeListeners[NonUseable]->write( addr, val );
    // IO ports
    else if( 0xFF00 <= addr && addr <= 0xFF7F )
        writeListeners[IOPorts]->write( addr, val );
    // HRAM
    else if( 0xFF80 <= addr && addr <= 0xFFFE )
        writeListeners[HRam]->write( addr, val );
    // Interrupt Enable
    else if( addr == 0xFFFF )
        writeListeners[IReg]->write( addr, val );
}

void Memory::registerListener( AddressRange range, MemoryInterface* mem ) {
    registerReadListener( range, mem );
    registerWriteListener( range, mem );
}

void Memory::registerReadListener( AddressRange range, MemoryInterface* mem ) {
    readListeners[range] = mem;
}

void Memory::registerWriteListener( AddressRange range, MemoryInterface* mem ) {
    writeListeners[range] = mem;
}

IOMemory* Memory::getIOMemory()
{
    return ioMem;
}
