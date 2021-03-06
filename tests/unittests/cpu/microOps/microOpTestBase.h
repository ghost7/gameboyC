#ifndef __MICRO_OP_TEST_BASE__
#define __MICRO_OP_TEST_BASE__

#include "../../../include/gtest/gtest.h"
#include "../../src/Memory/MemoryInterface.h"
#include "../../src/Cpu/Z80InstructionSet.h"

#define MEM_SIZE 0xFFFF

class MemoryEmulator : public MemoryInterface
{
public:
    uint8_t read(addr_t memAddr);
    void write(addr_t memAddr, data_t data);

    void clearMemory();
    MemoryEmulator();
private:
    data_t memory[MEM_SIZE];
};

/**
 * Base class for MicroOP tests.
 */
class MicroOpTestBase : public ::testing::Test
{
protected:
    /**
     * Test set up.
     */
    virtual void SetUp();

    /**
     * Test clean up.
     */
    virtual void TearDown();
    
    /**
     * Gets a random memory address.
     * 
     * \return a random memory address.
     */
    uint16_t GetMemoryAddress();
    
    /**
     * Gets a random number that can be used as for the contentes of a memory
     * address or a register.
     *
     * \param a ranodom 8-bit number.
     */
    uint8_t GetRandomNumber();
    
    /**
     * Loads a memory address into a 16 bit register
     * 
     * \param highReg the upper 8 bits of the register.
     * \param lowReg the lower 8 bits of the register.
     * \return the memory address stored into the register.
     */
    uint16_t LoadMemoryReg(RegisterPair *regPair);
    
    /**
     * Get a 16-bit value from a regiter.
     *
     * \param highReg the upper 8 bits of the register.
     * \param lowReg the lower 8 bits of the register.
     * \return 16-bit value stored in the register.
     */
    uint16_t Get16BitRegister(uint8_t highReg, uint8_t lowReg);
    
    /**
     * Sets the value for a 16-bit register.
     *
     * \param highReg the upper 8 bits of the register.
     * \param lowReg the lower 8 bits of the register.
     * \param value the value to bestored in the register.
     */
    void Set16BitRegister(uint8_t *highReg, uint8_t *lowReg, uint16_t value);

    /**
     * Sets the next immediate value to be read to the specified 8-bit value.
     * Note this changes the PC.
     *
     * \param value 8-bit value to be the next immediate value.
     */
    void SetImmValue(uint8_t value);
    
    /**
     * Sets the next immediate value to be read to the specified 16-bit value.
     * Note this changes the PC.
     *
     * \param value 16-bit value to be the next immediate value.
     */
    void SetImm16Value(uint16_t value);

    /**
     * Stores a value into memory.
     *
     * \param memoryAddress the memory address in which to store the data.
     * \param data the value to store.
     */
    void SetMemory(uint16_t memoryAddress, uint8_t data);
    
    /**
     * Gets the memory contents at the specifed address.
     *
     * \param memoryAddress the memory address in which to retrieve the data.
     */
    uint8_t GetMemory(uint16_t memoryAddress);
    
    /* Test Registers */
    Z80Registers registers;
    /* Test Flags */
    Z80Flags *flags;

    Z80InstructionSet *instSet;

    MemoryEmulator memory;
};

#endif
