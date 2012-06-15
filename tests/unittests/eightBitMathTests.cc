#include "microOpTestBase.h"

/**
 * Eight-Bit Arithmetic Tests.
 */
class EightBitMathTest : public ::MicroOpTestBase
{

};

TEST_F(EightBitMathTest, AddRegTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            addReg(j);
            ASSERT_EQ((uint8_t)(i + j), registers.A) << i << " + " << j;
        }
    }
}

TEST_F(EightBitMathTest, AddImmTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            SetImmValue(j);
            addImm();
            ASSERT_EQ((uint8_t)(i + j), registers.A) << i << " + " << j;
        }
    }
}

TEST_F(EightBitMathTest, AddHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
            registers.A = i;
            SetMemory(memoryAddress, j);
            addHLInd();
            ASSERT_EQ((uint8_t)(i + j), registers.A) << i << " + " << j;
        }
    }
}

TEST_F(EightBitMathTest, AdcRegTest)
{
     for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            flags.C = 1;
            adcReg(j);
            ASSERT_EQ((uint8_t)(i + j + 1), registers.A) << i << " + " << j << " + 1";
            registers.A = i;
            flags.C = 0;
            adcReg(j);
            ASSERT_EQ((uint8_t)(i + j), registers.A) << i << " + " << j << " + 0";
        }
    }
}

TEST_F(EightBitMathTest, AdcImmTest)
{
     for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            SetImmValue(j);
            flags.C = 1;
            adcImm();
            ASSERT_EQ((uint8_t)(i + j + 1), registers.A) << i << " + " << j << " + 1";
            registers.A = i;
            SetImmValue(j);
            flags.C = 0;
            adcImm();
            ASSERT_EQ((uint8_t)(i + j), registers.A) << i << " + " << j << " + 0";
        }
    }
}

TEST_F(EightBitMathTest, AdcHLIndTest)
{
     for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
            SetMemory(memoryAddress, j);

            registers.A = i;
            flags.C = 1;
            adcHLInd();
            ASSERT_EQ((uint8_t)(i + j + 1), registers.A) << i << " + " << j << " + 1";
            registers.A = i;
            flags.C = 0;
            adcHLInd();
            ASSERT_EQ((uint8_t)(i + j), registers.A) << i << " + " << j << " + 0";
        }
    }
}

TEST_F(EightBitMathTest, SubRegTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            subReg(j);
            ASSERT_EQ((uint8_t)(i - j), registers.A) << i << " - " << j;
        }
    }
}

TEST_F(EightBitMathTest, SubImmTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            SetImmValue(j);
            subImm();
            ASSERT_EQ((uint8_t)(i - j), registers.A) << i << " - " << j;
        }
    }
}

TEST_F(EightBitMathTest, SubHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
            registers.A = i;
            SetMemory(memoryAddress, j);
            subHLInd();
            ASSERT_EQ((uint8_t)(i - j), registers.A) << i << " - " << j;
        }
    }
}

TEST_F(EightBitMathTest, SbcRegTest)
{
     for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            flags.C = 1;
            sbcReg(j);
            ASSERT_EQ((uint8_t)(i - j - 1), registers.A) << i << " - " << j << " - 1";
            registers.A = i;
            flags.C = 0;
            sbcReg(j);
            ASSERT_EQ((uint8_t)(i - j), registers.A) << i << " - " << j << " - 0";
        }
    }
}

TEST_F(EightBitMathTest, SbcImmTest)
{
     for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            SetImmValue(j);
            flags.C = 1;
            sbcImm();
            ASSERT_EQ((uint8_t)(i - j - 1), registers.A) << i << " - " << j << " - 1";
            registers.A = i;
            SetImmValue(j);
            flags.C = 0;
            sbcImm();
            ASSERT_EQ((uint8_t)(i - j), registers.A) << i << " - " << j << " - 0";
        }
    }
}

TEST_F(EightBitMathTest, SbcHLIndTest)
{
     for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
            SetMemory(memoryAddress, j);

            registers.A = i;
            flags.C = 1;
            sbcHLInd();
            ASSERT_EQ((uint8_t)(i - j - 1), registers.A) << i << " - " << j << " - 1";
            registers.A = i;
            flags.C = 0;
            sbcHLInd();
            ASSERT_EQ((uint8_t)(i - j), registers.A) << i << " - " << j << " - 0";
        }
    }
}

TEST_F(EightBitMathTest, AndRegTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            andReg(j);
            ASSERT_EQ((uint8_t)(i & j), registers.A) << i << " and " << j;
        }
    }
}

TEST_F(EightBitMathTest, AndImmTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            SetImmValue(j);
            andImm();
            ASSERT_EQ((uint8_t)(i & j), registers.A) << i << " and " << j;
        }
    }
}

TEST_F(EightBitMathTest, AndHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
            registers.A = i;
            SetMemory(memoryAddress, j);
            andHLInd();
            ASSERT_EQ((uint8_t)(i & j), registers.A) << i << " and " << j;
        }
    }
}

TEST_F(EightBitMathTest, XorRegTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            xorReg(j);
            ASSERT_EQ((uint8_t)(i ^ j), registers.A) << i << " xor " << j;
        }
    }
}

TEST_F(EightBitMathTest, XorImmTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            SetImmValue(j);
            xorImm();
            ASSERT_EQ((uint8_t)(i ^ j), registers.A) << i << " xor " << j;
        }
    }
}

TEST_F(EightBitMathTest, XorHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
            registers.A = i;
            SetMemory(memoryAddress, j);
            xorHLInd();
            ASSERT_EQ((uint8_t)(i ^ j), registers.A) << i << " xor " << j;
        }
    }
}

TEST_F(EightBitMathTest, OrRegTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            orReg(j);
            ASSERT_EQ((uint8_t)(i | j), registers.A) << i << " or " << j;
        }
    }
}

TEST_F(EightBitMathTest, OrImmTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.A = i;
            SetImmValue(j);
            orImm();
            ASSERT_EQ((uint8_t)(i | j), registers.A) << i << " or " << j;
        }
    }
}

TEST_F(EightBitMathTest, OrHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
            registers.A = i;
            SetMemory(memoryAddress, j);
            orHLInd();
            ASSERT_EQ((uint8_t)(i | j), registers.A) << i << " or " << j;
        }
    }
}

TEST_F(EightBitMathTest, IncReg8Test)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.A = i;
        incReg8(&registers.A);
        ASSERT_EQ((uint8_t)(i + 1), registers.A) << i << " + 1";
    }
}

TEST_F(EightBitMathTest, IncHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
        SetMemory(memoryAddress, i);
        incHLInd();
        ASSERT_EQ((uint8_t)(i + 1), GetMemory(memoryAddress)) << i << " + 1";
    }
}

TEST_F(EightBitMathTest, DecReg8Test)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.A = i;
        decReg8(&registers.A);
        ASSERT_EQ((uint8_t)(i - 1), registers.A) << i << " - 1";
    }
}

TEST_F(EightBitMathTest, DecHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        uint16_t memoryAddress = LoadMemoryReg(&registers.H, &registers.L);
        SetMemory(memoryAddress, i);
        decHLInd();
        ASSERT_EQ((uint8_t)(i - 1), GetMemory(memoryAddress)) << i << " - 1";
    }
}

TEST_F(EightBitMathTest, ComplementATest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.A = i;
        complementA();
        ASSERT_EQ((uint8_t)(i ^ 0xFF), registers.A) << i << " ^ 0xFF";
    }
}
