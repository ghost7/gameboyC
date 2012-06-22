#include "../include/gtest/gtest.h"
#include "../../Z80.h"

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
    flags->Z = true;
    ASSERT_EQ(0x80, registers.AF.lo);
    flags->Z = false;
    ASSERT_EQ(0x00, registers.AF.hi);
}

/**
 * Test that setting and clearing the N flag correctly modifies the AF 
 * register.
 */
TEST_F(RegisterTest, NFlagTest)
{
    flags->N = true;
    ASSERT_EQ(0x40, registers.AF.lo);
    flags->N = false;
    ASSERT_EQ(0x00, registers.AF.hi);
}

/**
 * Test that setting and clearing the H flag correctly modifies the AF 
 * register.
 */
TEST_F(RegisterTest, HFlagTest)
{
    flags->H = true;
    ASSERT_EQ(0x20, registers.AF.lo);
    flags->H = false;
    ASSERT_EQ(0x00, registers.AF.hi);
}

/**
 * Test that setting and clearing the C flag correctly modifies the AF 
 * register.
 */
TEST_F(RegisterTest, CFlagTest)
{
    flags->C = true;
    ASSERT_EQ(0x10, registers.AF.lo);
    flags->C = false;
    ASSERT_EQ(0x00, registers.AF.hi);
}

/**
 * Test setting and clearing all the flags correctly modifies the AF 
 * register.
 */
TEST_F(RegisterTest, AllFlagTest)
{
    flags->Z = true;
    flags->N = true;
    flags->H = true;
    flags->C = true;
    ASSERT_EQ(0xF0, registers.AF.lo);

    flags->Z = false;
    flags->N = false;
    flags->H = false;
    flags->C = false;
    ASSERT_EQ(0x00, registers.AF.lo);
}
