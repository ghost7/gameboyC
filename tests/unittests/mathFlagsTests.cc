#include "flagOpTestBase.h"

/**
 * Tests arithmetic operations that modify flags.
 */
class MathFlagsTest : public ::FlagOpTestBase
{

};

/**
 * Tests if add operations set the correct flags.
 */
TEST_F(MathFlagsTest, Add8FlagTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            registers.B = j;
            addReg(registers.B);
            ClearAllFlags();
            if (i + j > 0xFF)
            {
                SetCTestFlag();
            }
            if ((uint8_t)(i + j) == 0)
            {
                SetZTestFlag();
            }
            if ((i & 0xF) + (j & 0xF) > 0xF)
            {
                SetHTestFlag();
            }
            ASSERT_NO_FATAL_FAILURE(CheckFlags()) << i << " + " << j;
        }
    }
}

/**
 * Tests if sub operations set the correct flags.
 */
TEST_F(MathFlagsTest, Sub8FlagTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            registers.B = j;
            subReg(registers.B);
            ClearAllFlags();
            if (i - j < 0)
            {
                SetCTestFlag();
            }
            if (i - j == 0)
            {
                SetZTestFlag();
            }
            if ((i & 0xF) - (j & 0xF) < 0)
            {
                SetHTestFlag();
            }
            SetNTestFlag();
            ASSERT_NO_FATAL_FAILURE(CheckFlags()) << i << " - " << j;
        }
    }
}

/**
 * Tests if and operations set the correct flags.
 */
TEST_F(MathFlagsTest, And8FlagTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            registers.B = j;
            andReg(registers.B);
            ClearAllFlags();
            if ((i & j) == 0)
            {
                SetZTestFlag();
            }
            SetHTestFlag();
            ASSERT_NO_FATAL_FAILURE(CheckFlags()) << i << " and " << j;
        }
    }
}

/**
 * Tests if xor operations set the correct flags.
 */
TEST_F(MathFlagsTest, Xor8FlagTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            registers.B = j;
            xorReg(registers.B);
            ClearAllFlags();
            if ((i ^ j) == 0)
            {
                SetZTestFlag();
            }
            ASSERT_NO_FATAL_FAILURE(CheckFlags()) << i << " xor " << j;
        }
    }
}

/**
 * Tests if or operations set the correct flags.
 */
TEST_F(MathFlagsTest, Or8FlagTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            registers.B = j;
            orReg(registers.B);
            ClearAllFlags();
            if ((i | j) == 0)
            {
                SetZTestFlag();
            }
            ASSERT_NO_FATAL_FAILURE(CheckFlags()) << i << " or " << j;
        }
    }
}

/**
 * Tests if compare operations set the correct flags.
 */
TEST_F(MathFlagsTest, CompareFlagTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            registers.B = j;
            compareReg(registers.B);
            ClearAllFlags();
            if (i - j < 0)
            {
                SetCTestFlag();
            }
            if (i - j == 0)
            {
                SetZTestFlag();
            }
            if ((i & 0xF) - (j & 0xF) < 0)
            {
                SetHTestFlag();
            }
            SetNTestFlag();
            ASSERT_NO_FATAL_FAILURE(CheckFlags()) << i << " - " << j;
        }
    }
}

/**
 * Tests if inc operations set the correct flags.
 */
TEST_F(MathFlagsTest, Inc8FlagTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.A = i;
        incReg8(&registers.A);
        ClearAllFlags();
        if ((uint8_t)(i + 1) == 0)
        {
            SetZTestFlag();
        }
        if ((i & 0xF) + 1 > 0xF)
        {
            SetHTestFlag();
        }
        ASSERT_NO_FATAL_FAILURE(CheckFlags()) << "inc " << i;
    }
}

/**
 * Tests if dec operations set the correct flags.
 */
TEST_F(MathFlagsTest, Dec8FlagTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.A = i;
        decReg8(&registers.A);
        ClearAllFlags();
        if ((uint8_t)(i - 1) == 0)
        {
            SetZTestFlag();
        }
        if ((i & 0xF) - 1 < 0)
        {
            SetHTestFlag();
        }
        SetNTestFlag();
        ASSERT_NO_FATAL_FAILURE(CheckFlags()) << "dec " << i;
    }
}

/**
 * Tests if dec operations set the correct flags.
 */
TEST_F(MathFlagsTest, Add16FlagTest)
{
    for (int i = 0; i < 0xFFFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            ClearAllFlags();
            testFlags.Z = flags.Z;
            
            Set16BitRegister(&registers.H, &registers.L, i);
            Set16BitRegister(&registers.B, &registers.C, j);
            addReg16(registers.B, registers.C);
            if (i + j > 0xFFFF)
            {
                SetCTestFlag();
            }
            if ((i & 0xFFF) + (j & 0xFFF) > 0xFFF)
            {
                SetHTestFlag();
            }

            ASSERT_NO_FATAL_FAILURE(CheckFlags()) << "16-bit: " << i << " + " << j;
        }
    }
}
