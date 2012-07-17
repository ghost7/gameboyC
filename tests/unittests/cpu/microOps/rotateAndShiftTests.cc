#include "microOpTestBase.h"

/**
 * Rotate and Shift Tests.
 */
class RotateAndShiftTest : public ::MicroOpTestBase
{
protected:
    /**
     * Gets the most significant bit of an eight-bit value.
     *
     * \param value the eight-bit value.
     */
    int GetMSB(uint8_t value)
    {
        return (value >> 7) & 0x1;
    }
    
    /**
     * Gets the least significat bit of an eight-bit value.
     *
     * \paran value The eight-bit value.
     */
    int GetLSB(uint8_t value)
    {
        return value & 0x1;
    }
    
    /**
     * Sets the most significat bit to 1.
     *
     * \param value Value to set.
     */
    void SetMSB(uint8_t *value)
    {
        *value |= 0x80;
    }
    
    /**
     * Sets the least significat bit to 1.
     *
     * \param value Value to set.
     */
    void SetLSB(uint8_t *value)
    {
        *value |= 0x01;
    }
    
    /**
     * Clears the most significant bit to 1.
     *
     * \param value Value to clear.
     */
    void ClearMSB(uint8_t *value)
    {
        *value &= 0x7F;
    }
    
    /**
     * Clears the least significant bit.
     *
     * \param value Value to clear.
     */
    void ClearLSB(uint8_t *value)
    {
        *value &= 0xFE;
    }

    uint8_t RotateLeft(uint8_t value)
    {
        int msb = GetMSB(value);
        uint8_t newValue = value << 1;
        return newValue |= msb;
    }

    uint8_t RotateRight(uint8_t value)
    {
        int lsb = GetLSB(value);
        uint8_t newValue = value >> 1;
        if (lsb == 1)
        {
            SetMSB(&newValue);
        }
        else
        {
            ClearMSB(&newValue);
        }
        return newValue;
    }

    uint8_t ShiftLeft(uint8_t value)
    {
        return value << 1;
    }

    uint8_t ShiftRight(uint8_t value)
    {
        return (value >> 1) & 0x7F;
    }

    void CheckCarryMSB(uint8_t value)
    {
        ASSERT_EQ((unsigned)GetMSB(value), flags->C);
    }

    void CheckCarryLSB(uint8_t value)
    {
        ASSERT_EQ((unsigned)GetLSB(value), flags->C);
    }
};

TEST_F(RotateAndShiftTest, RotateALeftCTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.AF.hi = i;
        instSet->rotateALeftC();
        ASSERT_EQ(RotateLeft(i), registers.AF.hi);
        // Make sure the msb went into the carry.
        ASSERT_NO_FATAL_FAILURE(CheckCarryMSB(i));
    }
}

TEST_F(RotateAndShiftTest, RotateALeftTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        flags->C = 1;
        registers.AF.hi = i;
        instSet->rotateALeft();
        // make sure the msb went into the carry.
        ASSERT_NO_FATAL_FAILURE(CheckCarryMSB(i))
            << i;
        // Make sure the old carry went into the lsb.
        ASSERT_EQ(1, GetLSB(registers.AF.hi))
            << i;
    
        flags->C = 0;
        registers.AF.hi = i;
        instSet->rotateALeft();
        // make sure the msb went into the carry
        ASSERT_NO_FATAL_FAILURE(CheckCarryMSB(i))
            << i;
        // Make sure the old carry went into the lsb.
        ASSERT_EQ(0, GetLSB(registers.AF.hi))
            << i;
    }
}

TEST_F(RotateAndShiftTest, RotateARightCTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.AF.hi = i;
        instSet->rotateARightC();
        ASSERT_EQ(RotateRight(i), registers.AF.hi);
        // Make sure the lsb went into the carry.
        ASSERT_NO_FATAL_FAILURE(CheckCarryLSB(i));
    }
}

TEST_F(RotateAndShiftTest, RotateARightTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        flags->C = 1;
        registers.AF.hi = i;
        instSet->rotateARight();
        // Make sure the lsb went into the carry.
        ASSERT_NO_FATAL_FAILURE(CheckCarryLSB(i))
            << i;
        // Make sure the old carry went into the msb.
        ASSERT_EQ(1, GetMSB(registers.AF.hi))
            << i;
    
        flags->C = 0;
        registers.AF.hi = i;
        instSet->rotateARight();
        // Make sure the lsb went into the carry.
        ASSERT_NO_FATAL_FAILURE(CheckCarryLSB(i))
            << i;
        // Make sure the old carry went into the msb.
        ASSERT_EQ(0, GetMSB(registers.AF.hi))
            << i;
    }
}

TEST_F(RotateAndShiftTest, RotateRegLeftCTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.BC.hi = i;
        Z80HalfRegisters hRegisters(registers);
        instSet->rotateRegLeftC(&hRegisters.B);
        ASSERT_EQ(RotateLeft(i), hRegisters.B);
        // Make sure the msb went into the carry.
        ASSERT_NO_FATAL_FAILURE(CheckCarryMSB(i));
    }
}

TEST_F(RotateAndShiftTest, RotateRegLeftTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        flags->C = 1;
        registers.BC.hi = i;
        Z80HalfRegisters hRegisters(registers);
        instSet->rotateRegLeft(&hRegisters.B);
        // make sure the msb went into the carry.
        ASSERT_NO_FATAL_FAILURE(CheckCarryMSB(i))
            << i;
        // Make sure the old carry went into the lsb.
        ASSERT_EQ(1, GetLSB(hRegisters.B))
            << i;
    
        flags->C = 0;
        registers.BC.hi = i;
        hRegisters = Z80HalfRegisters(registers);
        instSet->rotateRegLeft(&hRegisters.B);
        // make sure the msb went into the carry
        ASSERT_NO_FATAL_FAILURE(CheckCarryMSB(i))
            << i;
        // Make sure the old carry went into the lsb.
        ASSERT_EQ(0, GetLSB(hRegisters.B))
            << i;
    }
}

TEST_F(RotateAndShiftTest, RotateRegRightCTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.BC.hi = i;
        Z80HalfRegisters hRegisters(registers);
        instSet->rotateRegRightC(&hRegisters.B);
        ASSERT_EQ(RotateRight(i), hRegisters.B);
        // Make sure the lsb went into the carry.
        ASSERT_NO_FATAL_FAILURE(CheckCarryLSB(i));
    }
}

TEST_F(RotateAndShiftTest, RotateRegRightTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        flags->C = 1;
        registers.BC.hi = i;
        Z80HalfRegisters hRegisters(registers);
        instSet->rotateRegRight(&hRegisters.B);
        // Make sure the lsb went into the carry.
        ASSERT_NO_FATAL_FAILURE(CheckCarryLSB(i))
            << i;
        // Make sure the old carry went into the msb.
        ASSERT_EQ(1, GetMSB(hRegisters.B))
            << i;
    
        flags->C = 0;
        hRegisters.B = i;
        instSet->rotateRegRight(&hRegisters.B);
        // Make sure the lsb went into the carry.
        ASSERT_NO_FATAL_FAILURE(CheckCarryLSB(i))
            << i;
        // Make sure the old carry went into the msb.
        ASSERT_EQ(0, GetMSB(hRegisters.B))
            << i;
    }
}

TEST_F(RotateAndShiftTest, ShiftRegLeftATest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.BC.hi = i;
        Z80HalfRegisters hRegisters(registers);
        instSet->shiftRegLeftA(&hRegisters.B);
        ASSERT_EQ(ShiftLeft(i), hRegisters.B);
        // Make sure the msb went into the carry.
        ASSERT_NO_FATAL_FAILURE(CheckCarryMSB(i));
    }
}

TEST_F(RotateAndShiftTest, SwapRegTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.BC.hi = i;
        int highNibble = (i >> 4) & 0xF;
        int lowNibble = i & 0xF;
        int expectedValue = lowNibble << 4 | highNibble;
        
        Z80HalfRegisters hRegisters(registers);
        instSet->swapReg(&hRegisters.B);
        ASSERT_EQ(expectedValue, hRegisters.B) << i;
    }
}

TEST_F(RotateAndShiftTest, ShiftRegRightATest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.BC.hi = i;
        Z80HalfRegisters hRegisters(registers);
        instSet->shiftRegRightA(&hRegisters.B);

        // The 7th bit stays the same
        int expectedValue = ShiftRight(i) | (i & 0x80);
        ASSERT_EQ(expectedValue, hRegisters.B) << i;
        // Make sure the lsb went into the carry.
        ASSERT_NO_FATAL_FAILURE(CheckCarryLSB(i)) << i;
    }
}

TEST_F(RotateAndShiftTest, ShiftRegRightLTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.BC.hi = i;
        Z80HalfRegisters hRegisters(registers);
        instSet->shiftRegRightL(&hRegisters.B);

        // The 7th bit goes to 0.
        ASSERT_EQ(ShiftRight(i), hRegisters.B) << i;
        // Make sure the lsb went into the carry.
        ASSERT_NO_FATAL_FAILURE(CheckCarryLSB(i)) << i;
    }
}
