#include "memoryTestBase.h"

/**
 * This tests MBC dependant features in the case that there is no MBC 
 * on the cartridge.
 * TODO: Is this the best way?
 * Since ROM can't be written to, I had to assume certain properties
 * about the 'ROM' that gets loaded for testing. Specifically,
 * I assume that a memory bank n will be filled entirely with the
 * value n.
 *
 * The test adjusts to the following values, learned by reading from 
 * the cartridge using the 'read' function.
 * Target Cart Types(0x0147): 0x00, 0x08, 0x09
 * Target ROM Sizes(0x0148): 0x00
 * Target RAM Sizes(0x0149): 0x00, 0x01, 0x02
 *
 * \author Michael Cogswell
 */
class NoMbcMemoryTest : public ::MemoryTestBase {

// TODO: load cartridge
// TODO: parameterize with cartridge types: 0x00, 0x08, 0x09, 
// TODO: assert cart type is compatible with test (has header)

};

/********************************************
 * 0000 - 3FFF 
 * ROM bank 0 - Writing here may change the RAM bank, but nothing else.
 *              The first half of the cartridge is here.
 ********************************************/
TEST_F( NoMbcMemoryTest, RomBank0Test ) {
    for( addr_t addr = 0x0000; addr <= 0x3FFF; addr++ ) {
        ASSERT_RO( addr )
        // these headers values are generated for the test carts
        if( 0x0147 <= addr && addr <= 0x0149 )
            continue;
        // ROM bank 0 should be filled with 0s
        ASSERT_EQ( 0, mem->read( addr ) );
    }
}

/********************************************
 * 4000 - 7FFF 
 * ROM banks - The second half of the cartridge is loaded here.
 ********************************************/
TEST_F( NoMbcMemoryTest, RomBank1Test ) {
    for( addr_t addr = 0x4000; addr <= 0x7FFF; addr++ ) {
        ASSERT_RO( addr )
        // 'ROM bank' 1 should be filled with 1s
        ASSERT_EQ( 1, mem->read( addr ) );
    }
}

/********************************************
 * A000 - BFFF 
 * External RAM - Enabled by writing 0x0A to an address in 
 *                [0x0000 - 0x1FFF]. While I couldn't find something 
 *                that said it does this without an MBC, this is the only
 *                way I have seen that RAM banks (on the cartridge) are to
 *                be enabled or disabled. Therefore, I just
 *                assumed it would be the same without an MBC. 
 *                I don't think there are many games which 
 *                don't require ROM banking, but do require RAM banking.
 ********************************************/
TEST_F( NoMbcMemoryTest, RamBankTest ) {
    // I'm only testing for enabled RAM banking because
    // there's no defined behavior when it's disabled.

    // ensure the cartridge supports RAM banking
    data_t ctype = mem->read( 0x0147 );
    // only test it if there's a possibility of external RAM
    if( ctype == 0x08 || ctype == 0x09 ) {
        // how much RAM is supported? (up to 8KB)
        data_t rs = mem->read( 0x0149 );
        int ramSize = 0;
        if( rs == 0x01 )
            ramSize = 2*KB_SIZE; // 2 KB
        else if( rs == 0x02 )
            ramSize = 8*KB_SIZE; // 8 KB

        // enable RAM banking
        mem->write( 0x0000, 0x0A );
        // make sure the bank is RW
        for( addr_t addr = 0xA000; addr < 0xA000+ramSize; addr++ ) {
            ASSERT_RW( addr )
        }
    }
}
