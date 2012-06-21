#include "memoryTestBase.h"

/**
 * This tests MBC dependant features in the case that the cartridge
 * contains an MBC2.
 * TODO: Is this the best way?
 * Since ROM can't be written to, I had to assume certain properties
 * about the 'ROM' that gets loaded for testing. Specifically,
 * I assume that a memory bank n will be filled entirely with the
 * value n (except 0, which may have different values in the header).
 *
 * The test adjusts to the following values, learned by reading from 
 * the cartridge using the 'read' function.
 * Target Cart Types(0x0147): 0x05, 0x06
 * Target ROM Sizes(0x0148): 0x00-0x03
 * Target RAM Sizes(0x0149): n/a
 *
 * @author Michael Cogswell
 */
class Mbc1MemoryTest : public ::MemoryTestBase {

protected:

    int romSize;
    int romBanks;

    void SetUp() {
        MemoryTestBase::SetUp();
        // TODO: load cartridge (all types?)
        // TODO: assert that it's a compatible type (header wise)
   
        // get the memory sizes
        data_t romType = read( 0x0148 );
        switch( romType ) {
            case 0x00: romSize = 32*KB_SIZE;
                break;
            case 0x01: romSize = 64*KB_SIZE;
                break;
            case 0x02: romSize = 128*KB_SIZE;
                break;
            case 0x03: romSize = 256*KB_SIZE;
                break;
            /* Not supported 
            case 0x04: romSize = 512*KB_SIZE;
                break;
            case 0x05: romSize = 1024*KB_SIZE;
                break;
            case 0x06: romSize = 2048*KB_SIZE;
                break;
            case 0x07: romSize = 4096*KB_SIZE;
                break; 
            case 0x52: romSize = 1152*KB_SIZE;
                break;
            case 0x53: romSize = 1280*KB_SIZE;
                break;
            case 0x54: romSize = 1536*KB_SIZE;
                break; */
            default: FAIL() << "ROM size not recognized!";
                break;
        }
        romBanks = romSize/16;
    }

    /** 
     * Switch to the given ROM bank.
     *
     * @param bank The number of the desired bank
     */
    void switchRomBank( int bank ) {
        write( 0x2100, b );
    }
};


/********************************************
 * 0000 - 3FFF 
 * ROM bank 0 - Read only test
 ********************************************/
TEST_F( Mbc1MemoryTest, RomBank0Test ) {
    for( addr_t addr = 0x0000; addr <= 0x3FFF; addr++ ) {
        // ROM bank 0 should be filled with 0s
        ASSERT_EQ( 0, read( addr ) );
        ASSERT_RO( addr )
    }
}

/********************************************
 * 4000 - 7FFF 
 * ROM banks - see pandocs
 ********************************************/
;lkjk;lj    test that writing to a wrong address(even) in 2000-3FFF to switch rom banks will cause failure

TEST_F( Mbc1MemoryTest, RomBank1Test ) {
    for( int bank = 0; bank < romBanks; bank++ ) {
        // these special cases are tested in WeirdRomBankTest
        if( bank == 0 || bank == 0x20 || bank == 0x40 || bank == 0x60 )
            continue;

        // switch to the bank
        switchRomBank( bank );

        // make sure it's read only and contains the right value (for this test case)
        for( addr_t addr = 0x4000; addr <= 0x7FFF; addr++ ) {
            ASSERT_RO( addr )
            // 'ROM bank' i should be filled with is
            ASSERT_EQ( bank, read( addr ) );
        }
    }
}
TEST_F( Mbc1MemoryTest, WeirdRomBankTest ) {
    for( int bank = 0; bank < romBanks; bank++ ) {
        // These banks automatically select their immediately following bank (i.e. 0x20 -> 0x21).
        // Ensure that the corresponding bank is selected by checking the value read,
        // which should equal the bank number.
        data_t val;
        switch( bank ) {
            case 0x00: val = 0x01;
                break;
            case 0x20: val = 0x21;
                break;
            case 0x40: val = 0x41;
                break;
            case 0x60: val = 0x61;
                break;
            default:
                continue;
        }
        for( addr_t addr = 0x4000; addr <= 0x7FFF; addr++ ) {
            ASSERT_EQ( val, read( addr ) );
        }
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
 *                I don't think there are many, if any, games which 
 *                don't require ROM banking, but do require RAM banking.
 ********************************************/
TEST_F( Mbc1MemoryTest, RamBankTest ) {
    // ensure the cartridge supports RAM banking
    data_t ctype = read( 0x0147 );
    if( ctype == 0x08 || ctype == 0x09 ) {
        // I'm only testing for enabled RAM banking because
        // there's no defined behavior when it's disabled.
        // (in this implementation it may even be the same as when enabled)
        write( 0x0000, 0x0A );

        // make sure the bank is RW
        for( addr_t addr = 0xA000; addr <= 0xBFFF; addr++ ) {
            ASSERT_RW( addr )
        }
    }
    else {
        FAIL() << "Delete this line once this tests real things. I just didn't want it to succeed without anything real to test.";
    }
}
