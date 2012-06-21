#include "memoryTestBase.h"

/**
 * This tests ONLY functionality that is consistent across all memory 
 * controller types. 
 */
class AllMbcMemoryTest : public ::MemoryTestBase {

protected:
    
    addr_t rw_io_addrs[42]; // read/write
    addr_t wo_io_addrs[4]; // write only
    addr_t ro_io_addrs[1]; // read only
    addr_t r76w_io_addrs[2]; // can be written, only bits 7-6 can be read
    addr_t r6w_io_addrs[4]; // all write, read only 6

public:

    virtual void SetUp() { 
        MemoryTestBase::SetUp();
        // initialize the arrays of addresses with similar functionality
        rw_io_addrs = { 0xFF00, 0xFF01, 0xFF02, 0xFF05, 0xFF06, 0xFF07, 
            0xFF0F, 0xFF10, 0xFF12, 0xFF17, 0xFF1B, 0xFF20, 0xFF21, 0xFF22, 0xFF24, 
            0xFF25, 0xFF30, 0xFF31, 0xFF32, 0xFF33, 0xFF34, 0xFF35, 0xFF36, 0xFF37, 
            0xFF38, 0xFF39, 0xFF3A, 0xFF3B, 0xFF3C, 0xFF3D, 0xFF3E, 0xFF3F, 0xFF40, 
            0xFF42, 0xFF43, 0xFF45, 0xFF47, 0xFF48, 0xFF49, 0xFF4A, 0xFF4B, 0xFFFF };
        wo_io_addrs = { 0xFF13, 0xFF18, 0xFF1D, 0xFF46 };
        ro_io_addrs = { 0xFF44 };
        r76w_io_addrs = { 0xFF11, 0xFF16 };
        r6w_io_addrs = { 0xFF14, 0xFF19, 0xFF1E, 0xFF23 };
    }

};

/********************************************
 * 0000 - 3FFF 
 * ROM bank 0 
 ********************************************/
// tested by MBC
// Reads aren't tested because then I'd have to load a cartridge, 
// which is already done for the other tests. 
/********************************************
 * 4000 - 7FFF 
 * ROM banks (changes with MBC)
 ********************************************/
// tested by MBC
/********************************************
 * 8000 - 9FFF 
 * Video RAM (switchable for GBC only)
 ********************************************/
TEST_F( AllMbcMemoryTest, DispEnabledVRamTest ) {
    // see the pandocs "LCD Status Register" and "Accessing VRAM and OAM" sections for details
    // The tested behavior should only occur when the display is enabled.
    mem->write( 0xFF40, 0xFF );
    // H-Blank
    mem->write( 0xFF41, 0 ); 
    ASSERT_EQ( 0, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0x8000; addr <= 0x9FFF; addr++ ) {
        ASSERT_RW( addr )
    }
    // V-Blank
    mem->write( 0xFF41, 1 );
    ASSERT_EQ( 1, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0x8000; addr <= 0x9FFF; addr++ ) {
        ASSERT_RW( addr )
    }
    // Searching OAM-RAM
    mem->write( 0xFF41, 2 );
    ASSERT_EQ( 2, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0x8000; addr <= 0x9FFF; addr++ ) {
        ASSERT_RW( addr )
    }
    // Transferring data to LCD driver
    mem->write( 0xFF41, 3 );
    ASSERT_EQ( 3, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0x8000; addr <= 0x9FFF; addr++ ) {
        // Although there is no requirement, our emulator should return 0xFF per the pandocs.
        ASSERT_EQ( 0xFF, mem->read( addr ) );
        mem->write( addr, 0x00 );
        ASSERT_EQ( 0xFF, mem->read( addr ) );
    }
}
TEST_F( AllMbcMemoryTest, DispDisabledVRamTest ) {
    // see the pandocs "LCD Status Register" and "Accessing VRAM and OAM" sections for details
    // I should always be able to read from VRAM when the display is disabled.
    mem->write( 0xFF40, 0x00 );
    // H-Blank
    mem->write( 0xFF41, 0 ); 
    ASSERT_EQ( 0, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0x8000; addr <= 0x9FFF; addr++ ) {
        ASSERT_RW( addr )
    }
    // V-Blank
    mem->write( 0xFF41, 1 );
    ASSERT_EQ( 1, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0x8000; addr <= 0x9FFF; addr++ ) {
        ASSERT_RW( addr )
    }
    // Searching OAM-RAM
    mem->write( 0xFF41, 2 );
    ASSERT_EQ( 2, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0x8000; addr <= 0x9FFF; addr++ ) {
        ASSERT_RW( addr )
    }
    // Transferring data to LCD driver
    mem->write( 0xFF41, 3 );
    ASSERT_EQ( 3, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0x8000; addr <= 0x9FFF; addr++ ) {
        ASSERT_RW( addr )
    }
}
/********************************************
 * A000 - BFFF 
 * External RAM (changes with MBC)
 ********************************************/
// tested by MBC
/********************************************
 * C000 - CFFF 
 * Working RAM (bank 0 for GBC)
 ********************************************/
TEST_F( AllMbcMemoryTest, WRam0Test ) {
    for( addr_t addr = 0xC000; addr <= 0xCFFF; addr++ ) {
        ASSERT_RW( addr )
    }
}
/********************************************
 * D000 - DFFF 
 * Working RAM continued (bank 1-7 for GBC)
 ********************************************/
TEST_F( AllMbcMemoryTest, WRam1_7Test ) {
    for( addr_t addr = 0xD000; addr <= 0xDFFF; addr++ ) {
        ASSERT_RW( addr )
    }
}
/********************************************
 * E000 - EFFF 
 * Echos working RAM C000 - CFFF
 ********************************************/
TEST_F( AllMbcMemoryTest, EchoRam0Test ) {
    for( addr_t addr = 0xE000; addr <= 0xEFFF; addr++ ) {
        ASSERT_RO( addr )
        // start defined in the ASSERT_RO macro
        mem->write( addr-0x2000, start+10 );
        ASSERT_EQ( start+10, mem->read( addr ) ) << "Writing to actual memory should be reflected in RAM echo.";
    }
}
/********************************************
 * F000 - FDFF 
 * Echos working RAM D000 - DDFF
 ********************************************/
TEST_F( AllMbcMemoryTest, EchoRam1_7Test ) {
    for( addr_t addr = 0xF000; addr <= 0xFDFF; addr++ ) {
        ASSERT_RO( addr )
        // start defined in the ASSERT_RO macro
        mem->write( addr-0x2000, start+10 );
        ASSERT_EQ( start+10, mem->read( addr ) ) << "Writing to actual memory should be reflected in RAM echo.";
    }
}
/********************************************
 * FE00 - FE9F 
 * Sprite Attribute Table (OAM - Object Attribute Memory)
 ********************************************/
TEST_F( AllMbcMemoryTest, DispEnabledOamTest ) {
    // see the pandocs "LCD Status Register" and "Accessing VRAM and OAM" sections for details
    // The tested behavior should only occur when the display is enabled.
    mem->write( 0xFF40, 0xFF );
    // H-Blank
    mem->write( 0xFF41, 0 ); 
    ASSERT_EQ( 0, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0xFE00; addr <= 0xFE9F; addr++ ) {
        ASSERT_RW( addr )
    }
    // V-Blank
    mem->write( 0xFF41, 1 );
    ASSERT_EQ( 1, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0xFE00; addr <= 0xFE9F; addr++ ) {
        ASSERT_RW( addr )
    }
    // Searching OAM-RAM
    mem->write( 0xFF41, 2 );
    ASSERT_EQ( 2, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0xFE00; addr <= 0xFE9F; addr++ ) {
        // Although there is no requirement, our emulator should return 0xFF per the pandocs.
        ASSERT_EQ( 0xFF, mem->read( addr ) );
        mem->write( addr, 0x00 );
        ASSERT_EQ( 0xFF, mem->read( addr ) );
    }
    // Transferring data to LCD driver
    mem->write( 0xFF41, 3 );
    ASSERT_EQ( 3, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0xFE00; addr <= 0xFE9F; addr++ ) {
        // Although there is no requirement, our emulator should return 0xFF per the pandocs.
        ASSERT_EQ( 0xFF, mem->read( addr ) );
        mem->write( addr, 0x00 );
        ASSERT_EQ( 0xFF, mem->read( addr ) );
    }
}
TEST_F( AllMbcMemoryTest, DispDisabledOamTest ) {
    // When the display is disabled, OAM and VRAM are always accessible
    mem->write( 0xFF40, 0x00 );
    // H-Blank
    mem->write( 0xFF41, 0 ); 
    ASSERT_EQ( 0, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0xFE00; addr <= 0xFE9F; addr++ ) {
        ASSERT_RW( addr )
    }
    // V-Blank
    mem->write( 0xFF41, 1 );
    ASSERT_EQ( 1, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0xFE00; addr <= 0xFE9F; addr++ ) {
        ASSERT_RW( addr )
    }
    // Searching OAM-RAM
    mem->write( 0xFF41, 2 );
    ASSERT_EQ( 2, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0xFE00; addr <= 0xFE9F; addr++ ) {
        ASSERT_RW( addr )
    }
    // Transferring data to LCD driver
    mem->write( 0xFF41, 3 );
    ASSERT_EQ( 3, mem->read( 0xFF41 ) ) << "I need to be able to change the LCD status register for testing.";
    for( addr_t addr = 0xFE00; addr <= 0xFE9F; addr++ ) {
        ASSERT_RW( addr )
    }
}
/********************************************
 * FEA0 - FEFF 
 * Not useable 
 ********************************************/
/********************************************
 * FF00 - FF7F 
 * I/O Ports - I don't care what they mean, just if I can write to them.
 * TODO: care what they mean 
 *       (test the state change effects in the feature implementation tests)
 ********************************************/
TEST_F( AllMbcMemoryTest, IOPortTest ) {

    data_t data;

    // read/write
    for( int i = 0; i < 42; i++ ) {
        ASSERT_RW( rw_io_addrs[i] )
    }

    // write only
    for( int i = 0; i < 4; i++ ) {
        // TODO: can't really test this without getting into the register functionality
        mem->write( wo_io_addrs[i], 0xFF );
    }

    // read only
    ASSERT_RO( ro_io_addrs[0] )

    // write all read 7-6 only
    for( int i = 0; i < 2; i++ ) {
        mem->write( r76w_io_addrs[i], 0x00 );
        data = mem->read( r76w_io_addrs[i] );
        ASSERT_EQ( 0, data >> 6 );
        // repeat to ensure the value written wasn't the initial value
        mem->write( r76w_io_addrs[i], 0xFF );
        data = mem->read( r76w_io_addrs[i] );
        ASSERT_EQ( 3, data >> 6 );
    }

    // write all read 6 only
    for( int i = 0; i < 4; i++ ) {
        mem->write( r6w_io_addrs[i], 0x00 );
        data = mem->read( r6w_io_addrs[i] );
        ASSERT_EQ( 0, 1 & (data >> 6) );
        // repeat to ensure the value written wasn't the initial value
        mem->write( r6w_io_addrs[i], 0xFF );
        data = mem->read( r6w_io_addrs[i] );
        ASSERT_EQ( 1, 1 & (data >> 6) );
    }

    // write sets to 0, read all
    mem->write( 0xFF04, 0x00 );
    ASSERT_EQ( 0, mem->read( 0xFF04 ) );
    mem->write( 0xFF04, 0xFF );
    ASSERT_EQ( 0, mem->read( 0xFF04 ) );

    // write all read 7 only
    mem->write( 0xFF1A, 0x00 );
    data = mem->read( 0xFF1A );
    ASSERT_EQ( 0, data >> 7 );
    // repeat to ensure the value written wasn't the initial value
    mem->write( 0xFF1A, 0xFF );
    data = mem->read( 0xFF1A );
    ASSERT_EQ( 1, data >> 7 );

    // write all, read 6-5 only
    mem->write( 0xFF1C, 0x00 );
    data = mem->read( 0xFF1C );
    ASSERT_EQ( 0, 3 & (data >> 5) );
    // repeat to ensure the value written wasn't the initial value
    mem->write( 0xFF1C, 0xFF );
    data = mem->read( 0xFF1C );
    ASSERT_EQ( 3, 3 & (data >> 5) );

    // write only 7-4, read all
    mem->write( 0xFF1C, 0x00 );
    data = mem->read( 0xFF1C );
    // all 4 top bits are 0
    ASSERT_TRUE( data < 0x10 ); 
    mem->write( 0xFF1C, 0xFF );
    data = mem->read( 0xFF1C );
    // the top 4 are 1s
    ASSERT_EQ( 0xF0, data & 0xF0 );

    // write only 7-3, read all
    mem->write( 0xFF41, 0x00 );
    data = mem->read( 0xFF41 );
    // all 5 top bits are 0
    ASSERT_TRUE( data < 0x08 ); 
    mem->write( 0xFF41, 0xFF );
    data = mem->read( 0xFF41 );
    // the top 5 are 1s
    ASSERT_EQ( 0xF8, data & 0xF8 );
}
/********************************************
 * FF80 - FFFE 
 * High RAM 
 ********************************************/
TEST_F( AllMbcMemoryTest, HRamTest ) {
    for( addr_t addr = 0xFF80; addr <= 0xFFFE; addr++ ) {
        ASSERT_RW( addr )
    }
}
