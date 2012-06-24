#include "microOpTestBase.h"

/**
 * Sixteen-Bit Arithmetic Tests.
 */
class SixteenBitMathTest : public ::MicroOpTestBase
{

};

TEST_F(SixteenBitMathTest, AddReg16Test)
{
    uint16_t testValue = 100;
    uint16_t testValue2 = 200;

    registers.BC = testValue;
    registers.HL = testValue2;
    
    instSet->addReg16(registers.BC);
    ASSERT_EQ(testValue + testValue2, registers.HL.val);
}

TEST_F(SixteenBitMathTest, IncReg16Test)
{
    uint16_t testValue = 100;

    registers.BC = testValue;
    instSet->incReg16(&registers.BC);

    ASSERT_EQ((uint16_t)(testValue + 1), registers.BC.val);
}

TEST_F(SixteenBitMathTest, DecReg16Test)
{
    uint16_t testValue = 100;
    
    registers.BC = testValue;
    instSet->decReg16(&registers.BC);

    ASSERT_EQ((uint16_t)(testValue - 1), registers.BC.val);
}

TEST_F(SixteenBitMathTest, AddImmToSPTest)
{
    uint8_t posImm = 50;
    int8_t negImm = -50;
    uint16_t testSPValue = 500;

    // Verify adding a positive number.
    SetImmValue(posImm);
    registers.SP = testSPValue;
    instSet->addImmToSP();
    ASSERT_EQ(testSPValue + posImm, registers.SP.val);

    // Verify adding a negative number.
    SetImmValue(negImm);
    registers.SP = testSPValue;
    instSet->addImmToSP();
    ASSERT_EQ(testSPValue + negImm, registers.SP.val);
}

TEST_F(SixteenBitMathTest, AddSPToHLTest)
{
    uint8_t imm = 50;
    uint16_t testSPValue = 500;

    SetImmValue(imm);
    registers.SP = testSPValue;
    instSet->addSPToHL();

    ASSERT_EQ(imm + testSPValue, registers.HL.val);
    ASSERT_EQ(registers.HL.val, registers.SP.val);
}
