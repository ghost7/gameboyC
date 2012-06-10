#include "microOpTestBase.h"

/**
 * Eight-Bit Operation Tests.
 */
class EightBitLoadTest : public ::MicroOpTestBase
{
protected:
    /**
     * Gets the address of an IO port.
     *
     * \param portNum the number of the IO port
     */
    uint16_t GetIOPort(uint8_t portNum)
    {
        return 0xFF00 + portNum;
    }
};

TEST_F(EightBitLoadTest, LoadReg8Test)
{
    registers.B = 16;
    loadReg8(&registers.B, &registers.C);
    ASSERT_EQ(registers.B, registers.C);
}

TEST_F(EightBitLoadTest, LoadReg8ImmTest)
{
    uint8_t imm = 20;
    SetImmValue(imm);
    loadImmReg8(&registers.B);
    ASSERT_EQ(imm, registers.B);
}

TEST_F(EightBitLoadTest, LoadReg8HLTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
    SetMemory(memoryAddress, 20);
    loadReg8HL(&registers.B);
    ASSERT_EQ(registers.B, GetMemory(memoryAddress));
}

TEST_F(EightBitLoadTest, StoreReg8HLTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
    registers.B = 60;
    storeReg8HL(registers.B);
    ASSERT_EQ(registers.B, GetMemory(memoryAddress));
}

TEST_F(EightBitLoadTest, StoreHLImmTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
    uint8_t imm = 20;
    SetImmValue(imm);
    storeHLImm();
    ASSERT_EQ(imm, GetMemory(memoryAddress));
}

TEST_F(EightBitLoadTest, LoadATest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.B, &registers.C);
    SetMemory(memoryAddress, 20);
    loadA(registers.B, registers.C);
    ASSERT_EQ(GetMemory(memoryAddress), registers.A);
}

TEST_F(EightBitLoadTest, LoadAIndTest)
{
    uint16_t memoryAddress = GetMemoryAddress();
    uint16_t imm = memoryAddress;
    SetMemory(memoryAddress, 20);
    SetImm16Value(imm);
    loadAInd();
    ASSERT_EQ(GetMemory(memoryAddress), registers.A);
}

TEST_F(EightBitLoadTest, StoreATest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.B, &registers.C);
    registers.A = 20;
    storeA(registers.B, registers.C);
    ASSERT_EQ(registers.A, GetMemory(memoryAddress));
}

TEST_F(EightBitLoadTest, StoreAIndTest)
{
    uint16_t memoryAddress = GetMemoryAddress();
    uint16_t imm = memoryAddress;
    SetImm16Value(imm); 
    registers.A = 20;
    storeAInd();
    ASSERT_EQ(registers.A, GetMemory(memoryAddress));
}

TEST_F(EightBitLoadTest, ReadIOPortNTest)
{
    uint8_t n = 10;
    uint16_t iOPort = GetIOPort(n);
    SetMemory(iOPort, 20);
    SetImmValue(n);
    readIOPortN();
    ASSERT_EQ(GetMemory(iOPort), registers.A);
}

TEST_F(EightBitLoadTest, WriteIOPortNTest)
{
    uint8_t n = 10;
    uint16_t iOPort = GetIOPort(n);
    registers.A = 20;
    SetImmValue(n);
    writeIOPortN();
    ASSERT_EQ(registers.A, GetMemory(iOPort));
}

TEST_F(EightBitLoadTest, ReadIOPortCTest)
{
    registers.C = 10;
    uint16_t iOPort = GetIOPort(registers.C);
    SetMemory(iOPort, 20);
    readIOPortC();
    ASSERT_EQ(GetMemory(iOPort), registers.A);
}

TEST_F(EightBitLoadTest, WriteIOPortCTest)
{
    registers.C = 10;
    uint16_t iOPort = GetIOPort(registers.C);
    registers.A = 20;
    writeIOPortC();
    ASSERT_EQ(registers.A, GetMemory(iOPort));
}

TEST_F(EightBitLoadTest, StoreIncrementTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
    uint16_t HL = Get16BitRegister(registers.H, registers.L);
    registers.A = 20;
    storeIncrement();

    // verify the store.
    ASSERT_EQ(registers.A, GetMemory(memoryAddress));
    // verify the increment.
    uint16_t HLPlus1 = Get16BitRegister(registers.H, registers.L);
    ASSERT_EQ(HL + 1, HLPlus1);
}

TEST_F(EightBitLoadTest, LoadIncrementTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
    uint16_t HL = Get16BitRegister(registers.H, registers.L);
    SetMemory(memoryAddress, 20);
    loadIncrement();

    // verify the store.
    ASSERT_EQ(GetMemory(memoryAddress), registers.A);
    // verify the increment.
    uint16_t HLPlus1 = Get16BitRegister(registers.H, registers.L);
    ASSERT_EQ(HL + 1, HLPlus1);
}

TEST_F(EightBitLoadTest, StoreDecrementTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
    uint16_t HL = Get16BitRegister(registers.H, registers.L);
    registers.A = 20;
    storeDecrement();

    // verify the store.
    ASSERT_EQ(registers.A, GetMemory(memoryAddress));
    // verify the increment.
    uint16_t HLMinus1 = Get16BitRegister(registers.H, registers.L);
    ASSERT_EQ(HL - 1, HLMinus1);
}

TEST_F(EightBitLoadTest, LoadDecrementTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
    uint16_t HL = Get16BitRegister(registers.H, registers.L);
    SetMemory(memoryAddress, 20);
    loadDecrement();

    // verify the store.
    ASSERT_EQ(GetMemory(memoryAddress), registers.A);
    // verify the increment.
    uint16_t HLMinus1 = Get16BitRegister(registers.H, registers.L);
    ASSERT_EQ(HL - 1, HLMinus1);
}
