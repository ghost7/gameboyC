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
    registers.BC.hi = 16;
    instSet->loadReg8(&registers.BC.hi, &registers.BC.lo);
    ASSERT_EQ(registers.BC.hi, registers.BC.lo);
}

TEST_F(EightBitLoadTest, LoadReg8ImmTest)
{
    uint8_t imm = 20;
    SetImmValue(imm);
    instSet->loadImmReg8(&registers.BC.hi);
    ASSERT_EQ(imm, registers.BC.hi);
}

TEST_F(EightBitLoadTest, LoadReg8HLTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.HL);
    SetMemory(memoryAddress, 20);
    instSet->loadReg8HL(&registers.BC.hi);
    ASSERT_EQ(registers.BC.hi, GetMemory(memoryAddress));
}

TEST_F(EightBitLoadTest, StoreReg8HLTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.HL);
    registers.BC.hi = 60;
    instSet->storeReg8HL(registers.BC.hi);
    ASSERT_EQ(registers.BC.hi, GetMemory(memoryAddress));
}

TEST_F(EightBitLoadTest, StoreHLImmTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.HL);
    uint8_t imm = 20;
    SetImmValue(imm);
    instSet->storeHLImm();
    ASSERT_EQ(imm, GetMemory(memoryAddress));
}

TEST_F(EightBitLoadTest, LoadATest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.BC);
    SetMemory(memoryAddress, 20);
    instSet->loadA(registers.BC);
    ASSERT_EQ(GetMemory(memoryAddress), registers.AF.hi);
}

TEST_F(EightBitLoadTest, LoadAIndTest)
{
    uint16_t memoryAddress = GetMemoryAddress();
    uint16_t imm = memoryAddress;
    SetMemory(memoryAddress, 20);
    SetImm16Value(imm);
    instSet->loadAInd();
    ASSERT_EQ(GetMemory(memoryAddress), registers.AF.hi);
}

TEST_F(EightBitLoadTest, StoreATest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.BC);
    registers.AF.hi = 20;
    instSet->storeA(registers.BC);
    ASSERT_EQ(registers.AF.hi, GetMemory(memoryAddress));
}

TEST_F(EightBitLoadTest, StoreAIndTest)
{
    uint16_t memoryAddress = GetMemoryAddress();
    uint16_t imm = memoryAddress;
    SetImm16Value(imm); 
    registers.AF.hi = 20;
    instSet->storeAInd();
    ASSERT_EQ(registers.AF.hi, GetMemory(memoryAddress));
}

TEST_F(EightBitLoadTest, ReadIOPortNTest)
{
    uint8_t n = 10;
    uint16_t iOPort = GetIOPort(n);
    SetMemory(iOPort, 20);
    SetImmValue(n);
    instSet->readIOPortN();
    ASSERT_EQ(GetMemory(iOPort), registers.AF.hi);
}

TEST_F(EightBitLoadTest, WriteIOPortNTest)
{
    uint8_t n = 10;
    uint16_t iOPort = GetIOPort(n);
    registers.AF.hi = 20;
    SetImmValue(n);
    instSet->writeIOPortN();
    ASSERT_EQ(registers.AF.hi, GetMemory(iOPort));
}

TEST_F(EightBitLoadTest, ReadIOPortCTest)
{
    registers.BC.lo = 10;
    uint16_t iOPort = GetIOPort(registers.BC.lo);
    SetMemory(iOPort, 20);
    instSet->readIOPortC();
    ASSERT_EQ(GetMemory(iOPort), registers.AF.hi);
}

TEST_F(EightBitLoadTest, WriteIOPortCTest)
{
    registers.BC.lo = 10;
    uint16_t iOPort = GetIOPort(registers.BC.lo);
    registers.AF.hi = 20;
    instSet->writeIOPortC();
    ASSERT_EQ(registers.AF.hi, GetMemory(iOPort));
}

TEST_F(EightBitLoadTest, StoreIncrementTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.HL); 
    uint16_t HL = registers.HL.val;
    registers.AF.hi = 20;
    instSet->storeIncrement();

    // verify the instSet->store.
    ASSERT_EQ(registers.AF.hi, GetMemory(memoryAddress));
    // verify the increment.
    uint16_t HLPlus1 = registers.HL.val;
    ASSERT_EQ(HL + 1, HLPlus1);
}

TEST_F(EightBitLoadTest, LoadIncrementTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.HL); 
    uint16_t HL = registers.HL.val;
    SetMemory(memoryAddress, 20);
    instSet->loadIncrement();

    // verify the instSet->store.
    ASSERT_EQ(GetMemory(memoryAddress), registers.AF.hi);
    // verify the increment.
    uint16_t HLPlus1 = registers.HL.val;
    ASSERT_EQ(HL + 1, HLPlus1);
}

TEST_F(EightBitLoadTest, StoreDecrementTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.HL);
    uint16_t HL = registers.HL.val; 
    registers.AF.hi = 20;
    instSet->storeDecrement();

    // verify the instSet->store.
    ASSERT_EQ(registers.AF.hi, GetMemory(memoryAddress));
    // verify the increment.
    uint16_t HLMinus1 = registers.HL.val;
    ASSERT_EQ(HL - 1, HLMinus1);
}

TEST_F(EightBitLoadTest, LoadDecrementTest)
{
    uint16_t memoryAddress = LoadMemoryReg(&registers.HL); 
    uint16_t HL = registers.HL.val;
    SetMemory(memoryAddress, 20);
    instSet->loadDecrement();

    // verify the instSet->store.
    ASSERT_EQ(GetMemory(memoryAddress), registers.AF.hi);
    // verify the increment.
    uint16_t HLMinus1 = registers.HL.val;
    ASSERT_EQ(HL - 1, HLMinus1);
}
