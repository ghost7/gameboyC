#include "memoryTestBase.h"

/**
 * This tests MBC dependant features in the case that the cartridge
 * contains an MBC1.
 * TODO: Is this the best way?
 * Since ROM can't be written to, I had to assume certain properties
 * about the 'ROM' that gets loaded for testing. Specifically,
 * I assume that a memory bank n will be filled entirely with the
 * value n.
 *
 * The test adjusts to the following values, learned by reading from 
 * the cartridge using the 'read' function.
 * Target Cart Types(0x0147): 0x01, 0x02, 0x03
 * Target ROM Sizes(0x0148): 0x00-0x06, 0x52-0x54
 * Target RAM Sizes(0x0149): 0x00-0x03
 *
 * @author Michael Cogswell
 */
class Mbc1MemoryTest : public ::MemoryTestBase,
                       public ::testing::WithParamInterface<BankingMode> {

protected:

    int ramSize;
    int ramBanks;
    int romSize;
    int romBanks;

    void SetUp() {
        MemoryTestBase::SetUp();
        // TODO: load cartridge (all types?)
        // TODO: assert that it's a compatible type (header wise)
   
        // set the banking mode
        mem->write( 0x6000, GetParam() );
        // get the memory sizes and adjust for the mode
        data_t romType = mem->read( 0x0148 );
        switch( romType ) {
            case 0x00: romSize = 32*KB_SIZE;
                break;
            case 0x01: romSize = 64*KB_SIZE;
                break;
            case 0x02: romSize = 128*KB_SIZE;
                break;
            case 0x03: romSize = 256*KB_SIZE;
                break;
            case 0x04: romSize = 512*KB_SIZE;
                break;
            case 0x05: romSize = 1024*KB_SIZE;
                break;
            case 0x06: romSize = 2048*KB_SIZE;
                break;
            /* Not supported 
            case 0x07: romSize = 4096*KB_SIZE;
                break; */
            case 0x52: romSize = 1152*KB_SIZE;
                break;
            case 0x53: romSize = 1280*KB_SIZE;
                break;
            case 0x54: romSize = 1536*KB_SIZE;
                break;
            default: FAIL() << "ROM size not recognized!";
                break;
        }
        data_t ramType = mem->read( 0x0149 );
        switch( ramType ) {
            case 0x00: ramSize = 0;
                break;
            case 0x01: ramSize = 2*KB_SIZE;
                break;
            case 0x02: ramSize = 8*KB_SIZE;
                break;
            case 0x03: ramSize = 32*KB_SIZE;
                break;
            default: FAIL() << "RAM size not recognized!";
                break;
        }
        if( GetParam() == BankingMode::ROM_BANKING_MODE )
            ramSize = (ramSize > 8*KB_SIZE) ? 8*KB_SIZE : ramSize;
        else if( GetParam() == BankingMode::RAM_BANKING_MODE )
            romSize = (romSize > 512*KB_SIZE) ? 512*KB_SIZE : romSize;
        ramBanks = ramSize/(8*KB_SIZE);
        romBanks = romSize/(16*KB_SIZE);
    }


    /** 
     * Switch to the given ROM bank.
     *
     * @param bank The number of the desired bank
     */
    void switchRomBank( int bank ) {
        int b = 0x7F & bank;
        mem->write( 0x2000, b & 0x1F );
        mem->write( 0x4000, b >> 5 );
    }

    /**
     * Switch to the given RAM bank.
     * TODO: put this where RAM banking is actually used
     *
     * @param bank The RAM bank to use
     */
    void switchRamBank( int bank ) {
        int b = 0x3 & bank;
        mem->write( 0x4000, b );
    }
};


// need to call this to create the actual tests
INSTANTIATE_TEST_CASE_P( Mbc1MultipleMemoryModeTest, Mbc1MemoryTest, 
        ::testing::Values( BankingMode::ROM_BANKING_MODE, BankingMode::RAM_BANKING_MODE ) );


/********************************************
 * 0000 - 3FFF 
 * ROM bank 0 - Read only test
 ********************************************/
TEST_P( Mbc1MemoryTest, RomBank0Test ) {
    for( addr_t addr = 0x0000; addr <= 0x3FFF; addr++ ) {
        // ignore headers
        if( 0x0147 <= addr && addr <= 0x0149 )
            continue;
        // ROM bank 0 should be filled with 0s
        ASSERT_EQ( 0, mem->read( addr ) );
    }
}

/********************************************
 * 4000 - 7FFF 
 * ROM banks - see pandocs
 ********************************************/
TEST_P( Mbc1MemoryTest, RomBank1Test ) {
    for( int bank = 0; bank < romBanks; bank++ ) {
        // these special cases are tested in WeirdRomBankTest
        if( bank == 0 || bank == 0x20 || bank == 0x40 || bank == 0x60 )
            continue;

        switchRomBank( bank );

        // make sure it's read only and contains the right value (for this test case)
        for( addr_t addr = 0x4000; addr <= 0x7FFF; addr++ ) {
            // 'ROM bank' i should be filled with is
            ASSERT_EQ( bank, mem->read( addr ) );
        }
    }
}
TEST_P( Mbc1MemoryTest, WeirdRomBankTest ) {
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
        switchRomBank( bank );
        for( addr_t addr = 0x4000; addr <= 0x7FFF; addr++ ) {
            ASSERT_EQ( val, mem->read( addr ) );
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
TEST_P( Mbc1MemoryTest, RamBankTest ) {
    // I'm only testing for enabled RAM banking because
    // there's no defined behavior when it's disabled.
    // (in this implementation it may even be the same as when enabled)
    mem->write( 0x0000, 0x0A );
    for( int bank = 0; bank < ramBanks; bank++ ) {

        switchRamBank( bank );

        // make sure the bank is RW
        for( addr_t addr = 0xA000; addr <= 0xBFFF; addr++ ) {
            ASSERT_RW( addr )
        }
    }
}
