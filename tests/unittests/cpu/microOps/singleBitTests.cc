#include "microOpTestBase.h"

/**
 * Single-Bit Arithmetic Tests.
 */
class SingleBitTest : public ::MicroOpTestBase
{

};

TEST_F(SingleBitTest, TestRegBitTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.BC.hi = i;
        for (int j = 0; j < 7; j++)
        {
            int bitValue = (i >> j) & 0x1;
            instSet->testRegBit(&registers.BC.hi, j);
            if (bitValue == 0)
            {
                ASSERT_TRUE(flags->Z);
            }
            else
            {
                ASSERT_FALSE(flags->Z);
            }
            // negation flag always set to 0.
            ASSERT_FALSE(flags->N);
            // half carry flag always set to 1.
            ASSERT_TRUE(flags->H);
        }
    }
}

TEST_F(SingleBitTest, SetRegBitTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.BC.hi = i;
        for (int j = 0; j < 7; j++)
        {
            instSet->setRegBit(&registers.BC.hi, j);
            int bitValue = (registers.BC.hi >> j) & 0x1;
            ASSERT_EQ(1, bitValue);
        }
    }
}

TEST_F(SingleBitTest, ResetRegBitTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.BC.hi = i;
        for (int j = 0; j < 7; j++)
        {
            instSet->resetRegBit(&registers.BC.hi, j);
            int bitValue = (registers.BC.hi >> j) & 0x1;
            ASSERT_EQ(0, bitValue);
        }
    }
}
