#include "microOpTestBase.h"

/**
 * Sixteen-Bit Operation Tests.
 */
class SixteenBitLoadTest : public ::MicroOpTestBase
{

};

TEST_F(SixteenBitLoadTest, LoadReg16Test)
{
    uint16_t imm = 20;
    SetImm16Value(imm);
    instSet->loadReg16(&registers.BC);
    int16_t BC = registers.BC.val;
    ASSERT_EQ(imm, BC);
}

TEST_F(SixteenBitLoadTest, LoadSPImmTest)
{
    uint16_t imm = 20;
    SetImm16Value(imm);
    instSet->loadSPImm();
    ASSERT_EQ(imm, registers.SP.val);
}

TEST_F(SixteenBitLoadTest, LoadHLToSP)
{
    LoadMemoryReg(&registers.HL);
    instSet->loadHLToSP();
    ASSERT_EQ(registers.HL.val, registers.SP.val);
}

TEST_F(SixteenBitLoadTest, PushPopTests)
{
    registers.SP = GetMemoryAddress();
    registers.BC.val = GetMemoryAddress(); 
    instSet->pushToStack(registers.BC);
    instSet->popFromStack(&registers.DE);
    ASSERT_EQ(registers.BC.val, registers.DE.val);
}
