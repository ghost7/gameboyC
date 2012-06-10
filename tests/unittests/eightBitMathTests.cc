#include "flagOpTestBase.h"

class EightBitMathTest : public ::FlagOpTestBase
{

};

TEST_F(EightBitMathTest, AddZFlagTest)
{
    int testValue = 16;
    
    // Do an addition that will result in 0
    registers.A = testValue;
    registers.B = -testValue;
    addReg(registers.B);
    ASSERT_EQ(0, registers.A);
    // Zero flag should be set.
    SetZTestFlag();
    CheckFlags();

    // Do an addition that will result in > 0
    registers.A = testValue;
    registers.B = testValue;
    addReg(registers.B);
    ASSERT_EQ(testValue + testValue, registers.A);
    // Zero flag should be cleared.
    ClearZTestFlag();
    CheckFlags();
}
