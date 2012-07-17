#include "../../../include/gtest/gtest.h"
#include "../../src/Cpu/Z80.h"

/**
 * Tests Z80 registers an flags.
 */
class RegisterTest : public ::testing::Test
{
protected:
    void SetUp()
    {
        // Clear registers and get flags.
        memset(&registers, 0, sizeof(Z80Registers));
        flags = registers.getFlags();
    }
    /* Test registers. */
    Z80Registers registers;
    /* Test flags. */
    Z80Flags *flags;
};

/**
 * Test the union structure of the Z80Register. This means that if the 16-bit
 * value of the register pair is modifyed, it modifies the the upper and lower
 * 8-bit values correctly.
 */
TEST_F(RegisterTest, RegisterPairTest)
{
    uint8_t high = 0x65;
    uint8_t low = 0x47;

    registers.BC.val = high << 8 | low;
    Z80HalfRegisters hRegisters(registers);
    ASSERT_EQ(high, hRegisters.B);
    ASSERT_EQ(low, hRegisters.C);
}

/**
 * Test that setting and clearing the Z flag correctly modifies the AF 
 * register.
 */
TEST_F(RegisterTest, ZFlagTest)
{
    flags->Z = 1;
    uint8_t registerF = registers.AF.lo;
	ASSERT_EQ(0x80, registerF);
    flags->Z = 0;
	registerF = registers.AF.lo;
    ASSERT_EQ(0x00, registerF);
}

/**
 * Test that setting and clearing the N flag correctly modifies the AF 
 * register.
 */
TEST_F(RegisterTest, NFlagTest)
{
    flags->N = 1;
    uint8_t registerF = registers.AF.lo;
    ASSERT_EQ(0x40, registerF);
    flags->N = 0;
    registerF = registers.AF.lo;
    ASSERT_EQ(0x00, registerF);
}

/**
 * Test that setting and clearing the H flag correctly modifies the AF 
 * register.
 */
TEST_F(RegisterTest, HFlagTest)
{
    flags->H = 1;
    uint8_t registerF = registers.AF.lo;
    ASSERT_EQ(0x20, registerF);
    flags->H = 0;
    registerF = registers.AF.lo;
    ASSERT_EQ(0x00, registerF);
}

/**
 * Test that setting and clearing the C flag correctly modifies the AF 
 * register.
 */
TEST_F(RegisterTest, CFlagTest)
{
    flags->C = 1;
    uint8_t registerF = registers.AF.lo;
    ASSERT_EQ(0x10, registerF);
    flags->C = 0;
    registerF = registers.AF.lo;
    ASSERT_EQ(0x00, registerF);
}

/**
 * Test setting and clearing all the flags correctly modifies the AF 
 * register.
 */
TEST_F(RegisterTest, AllFlagTest)
{
    flags->Z = 1;
    flags->N = 1;
    flags->H = 1;
    flags->C = 1;
    uint8_t registerF = registers.AF.lo;
    ASSERT_EQ(0xF0, registerF);

    flags->Z = 0;
    flags->N = 0;
    flags->H = 0;
    flags->C = 0;
    registerF = registers.AF.lo;
    ASSERT_EQ(0x00, registerF);

    flags->Z = 1;
    int H = flags->H;
    ASSERT_EQ(H, 0);
}
