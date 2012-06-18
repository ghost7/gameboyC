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
            registers.AF.hi = i;
            instSet->addReg(j);
            ASSERT_EQ((uint8_t)(i + j), registers.AF.hi) << i << " + " << j;
        }
    }
}

TEST_F(EightBitMathTest, AddImmTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.AF.hi = i;
            SetImmValue(j);
            instSet->addImm();
            ASSERT_EQ((uint8_t)(i + j), registers.AF.hi) << i << " + " << j;
        }
    }
}

TEST_F(EightBitMathTest, AddHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.HL);
            registers.AF.hi = i;
            SetMemory(memoryAddress, j);
            instSet->addHLInd();
            ASSERT_EQ((uint8_t)(i + j), registers.AF.hi) << i << " + " << j;
        }
    }
}

TEST_F(EightBitMathTest, AdcRegTest)
{
     for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.AF.hi = i;
            flags->C = 1;
            instSet->adcReg(j);
            ASSERT_EQ((uint8_t)(i + j + 1), registers.AF.hi) << i << " + " << j << " + 1";
            registers.AF.hi = i;
            flags->C = 0;
            instSet->adcReg(j);
            ASSERT_EQ((uint8_t)(i + j), registers.AF.hi) << i << " + " << j << " + 0";
        }
    }
}

TEST_F(EightBitMathTest, AdcImmTest)
{
     for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.AF.hi = i;
            SetImmValue(j);
            flags->C = 1;
            instSet->adcImm();
            ASSERT_EQ((uint8_t)(i + j + 1), registers.AF.hi) << i << " + " << j << " + 1";
            registers.AF.hi = i;
            SetImmValue(j);
            flags->C = 0;
            instSet->adcImm();
            ASSERT_EQ((uint8_t)(i + j), registers.AF.hi) << i << " + " << j << " + 0";
        }
    }
}

TEST_F(EightBitMathTest, AdcHLIndTest)
{
     for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.HL);
            SetMemory(memoryAddress, j);

            registers.AF.hi = i;
            flags->C = 1;
            instSet->adcHLInd();
            ASSERT_EQ((uint8_t)(i + j + 1), registers.AF.hi) << i << " + " << j << " + 1";
            registers.AF.hi = i;
            flags->C = 0;
            instSet->adcHLInd();
            ASSERT_EQ((uint8_t)(i + j), registers.AF.hi) << i << " + " << j << " + 0";
        }
    }
}

TEST_F(EightBitMathTest, SubRegTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.AF.hi = i;
            instSet->subReg(j);
            ASSERT_EQ((uint8_t)(i - j), registers.AF.hi) << i << " - " << j;
        }
    }
}

TEST_F(EightBitMathTest, SubImmTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.AF.hi = i;
            SetImmValue(j);
            instSet->subImm();
            ASSERT_EQ((uint8_t)(i - j), registers.AF.hi) << i << " - " << j;
        }
    }
}

TEST_F(EightBitMathTest, SubHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.HL);
            registers.AF.hi = i;
            SetMemory(memoryAddress, j);
            instSet->subHLInd();
            ASSERT_EQ((uint8_t)(i - j), registers.AF.hi) << i << " - " << j;
        }
    }
}

TEST_F(EightBitMathTest, SbcRegTest)
{
     for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.AF.hi = i;
            flags->C = 1;
            instSet->sbcReg(j);
            ASSERT_EQ((uint8_t)(i - j - 1), registers.AF.hi) << i << " - " << j << " - 1";
            registers.AF.hi = i;
            flags->C = 0;
            instSet->sbcReg(j);
            ASSERT_EQ((uint8_t)(i - j), registers.AF.hi) << i << " - " << j << " - 0";
        }
    }
}

TEST_F(EightBitMathTest, SbcImmTest)
{
     for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.AF.hi = i;
            SetImmValue(j);
            flags->C = 1;
            instSet->sbcImm();
            ASSERT_EQ((uint8_t)(i - j - 1), registers.AF.hi) << i << " - " << j << " - 1";
            registers.AF.hi = i;
            SetImmValue(j);
            flags->C = 0;
            instSet->sbcImm();
            ASSERT_EQ((uint8_t)(i - j), registers.AF.hi) << i << " - " << j << " - 0";
        }
    }
}

TEST_F(EightBitMathTest, SbcHLIndTest)
{
     for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.HL);
            SetMemory(memoryAddress, j);

            registers.AF.hi = i;
            flags->C = 1;
            instSet->sbcHLInd();
            ASSERT_EQ((uint8_t)(i - j - 1), registers.AF.hi) << i << " - " << j << " - 1";
            registers.AF.hi = i;
            flags->C = 0;
            instSet->sbcHLInd();
            ASSERT_EQ((uint8_t)(i - j), registers.AF.hi) << i << " - " << j << " - 0";
        }
    }
}

TEST_F(EightBitMathTest, AndRegTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.AF.hi = i;
            instSet->andReg(j);
            ASSERT_EQ((uint8_t)(i & j), registers.AF.hi) << i << " instSet->and " << j;
        }
    }
}

TEST_F(EightBitMathTest, AndImmTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.AF.hi = i;
            SetImmValue(j);
            instSet->andImm();
            ASSERT_EQ((uint8_t)(i & j), registers.AF.hi) << i << " instSet->and " << j;
        }
    }
}

TEST_F(EightBitMathTest, AndHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.HL);
            registers.AF.hi = i;
            SetMemory(memoryAddress, j);
            instSet->andHLInd();
            ASSERT_EQ((uint8_t)(i & j), registers.AF.hi) << i << " instSet->and " << j;
        }
    }
}

TEST_F(EightBitMathTest, XorRegTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.AF.hi = i;
            instSet->xorReg(j);
            ASSERT_EQ((uint8_t)(i ^ j), registers.AF.hi) << i << " instSet->xor " << j;
        }
    }
}

TEST_F(EightBitMathTest, XorImmTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.AF.hi = i;
            SetImmValue(j);
            instSet->xorImm();
            ASSERT_EQ((uint8_t)(i ^ j), registers.AF.hi) << i << " instSet->xor " << j;
        }
    }
}

TEST_F(EightBitMathTest, XorHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.HL);
            registers.AF.hi = i;
            SetMemory(memoryAddress, j);
            instSet->xorHLInd();
            ASSERT_EQ((uint8_t)(i ^ j), registers.AF.hi) << i << " instSet->xor " << j;
        }
    }
}

TEST_F(EightBitMathTest, OrRegTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.AF.hi = i;
            instSet->orReg(j);
            ASSERT_EQ((uint8_t)(i | j), registers.AF.hi) << i << " or " << j;
        }
    }
}

TEST_F(EightBitMathTest, OrImmTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            registers.AF.hi = i;
            SetImmValue(j);
            instSet->orImm();
            ASSERT_EQ((uint8_t)(i | j), registers.AF.hi) << i << " or " << j;
        }
    }
}

TEST_F(EightBitMathTest, OrHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        for (int j = 0; j < 0xFF; j++)
        {
            uint16_t memoryAddress = LoadMemoryReg(&registers.HL);
            registers.AF.hi = i;
            SetMemory(memoryAddress, j);
            instSet->orHLInd();
            ASSERT_EQ((uint8_t)(i | j), registers.AF.hi) << i << " or " << j;
        }
    }
}

TEST_F(EightBitMathTest, IncReg8Test)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.AF.hi = i;
        instSet->incReg8(&registers.AF.hi);
        ASSERT_EQ((uint8_t)(i + 1), registers.AF.hi) << i << " + 1";
    }
}

TEST_F(EightBitMathTest, IncHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        uint16_t memoryAddress = LoadMemoryReg(&registers.HL);
        SetMemory(memoryAddress, i);
        instSet->incHLInd();
        ASSERT_EQ((uint8_t)(i + 1), GetMemory(memoryAddress)) << i << " + 1";
    }
}

TEST_F(EightBitMathTest, DecReg8Test)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.AF.hi = i;
        instSet->decReg8(&registers.AF.hi);
        ASSERT_EQ((uint8_t)(i - 1), registers.AF.hi) << i << " - 1";
    }
}

TEST_F(EightBitMathTest, DecHLIndTest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        uint16_t memoryAddress = LoadMemoryReg(&registers.HL);
        SetMemory(memoryAddress, i);
        instSet->decHLInd();
        ASSERT_EQ((uint8_t)(i - 1), GetMemory(memoryAddress)) << i << " - 1";
    }
}

TEST_F(EightBitMathTest, ComplementATest)
{
    for (int i = 0; i < 0xFF; i++)
    {
        registers.AF.hi = i;
        instSet->complementA();
        ASSERT_EQ((uint8_t)(i ^ 0xFF), registers.AF.hi) << i << " ^ 0xFF";
    }
}
