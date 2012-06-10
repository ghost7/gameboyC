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
    loadReg16(&registers.B, &registers.C);
    int16_t BC = Get16BitRegister(registers.B, registers.C);
    ASSERT_EQ(imm, BC);
}

TEST_F(SixteenBitLoadTest, LoadSPImmTest)
{
    uint16_t imm = 20;
    SetImm16Value(imm);
    loadSPImm();
    ASSERT_EQ(imm, registers.SP);
}

TEST_F(SixteenBitLoadTest, LoadHLToSP)
{
    LoadMemoryReg(&registers.H, &registers.L);
    loadHLToSP();
    int16_t HL = Get16BitRegister(registers.H, registers.L);
    ASSERT_EQ(HL, registers.SP);
}

TEST_F(SixteenBitLoadTest, PushPopTests)
{
    registers.SP = GetMemoryAddress();
    registers.B = 100;
    registers.C = 50;
    pushToStack(registers.B, registers.C);
    popFromStack(&registers.D, &registers.E);
    ASSERT_EQ(registers.B, registers.D);
    ASSERT_EQ(registers.C, registers.E);
}
