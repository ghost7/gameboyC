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
    ASSERT_EQ(high, registers.BC.hi);
    ASSERT_EQ(low, registers.BC.lo);
}

/**
 * Test that setting and clearing the Z flag correctly modifies the AF 
 * register.
 */
TEST_F(RegisterTest, ZFlagTest)
{
    flags->Z = 1;
	ASSERT_EQ(0x80, registers.AF.lo);
    flags->Z = 0;
	ASSERT_EQ(0x00, registers.AF.lo);
}

/**
 * Test that setting and clearing the N flag correctly modifies the AF 
 * register.
 */
TEST_F(RegisterTest, NFlagTest)
{
    flags->N = 1;
    ASSERT_EQ(0x40, registers.AF.lo);
    flags->N = 0;
    ASSERT_EQ(0x00, registers.AF.lo);
}

/**
 * Test that setting and clearing the H flag correctly modifies the AF 
 * register.
 */
TEST_F(RegisterTest, HFlagTest)
{
    flags->H = 1;
    ASSERT_EQ(0x20, registers.AF.lo);
    flags->H = 0;
    ASSERT_EQ(0x00, registers.AF.lo);
}

/**
 * Test that setting and clearing the C flag correctly modifies the AF 
 * register.
 */
TEST_F(RegisterTest, CFlagTest)
{
    flags->C = 1;
    ASSERT_EQ(0x10, registers.AF.lo);
    flags->C = 0;
    ASSERT_EQ(0x00, registers.AF.lo);
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
    ASSERT_EQ(0xF0, registers.AF.lo);

    flags->Z = 0;
    flags->N = 0;
    flags->H = 0;
    flags->C = 0;
    ASSERT_EQ(0x00, registers.AF.lo);
}
