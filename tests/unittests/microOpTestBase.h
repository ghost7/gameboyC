#ifndef __MICRO_OP_TEST_BASE__
#define __MICRO_OP_TEST_BASE__

#include "../include/gtest/gtest.h"
#include "../../cpu_z80_inst.h"
#include "../../registers.h"

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
     * Gets a random memory address.
     * 
     * \return a random memory address.
     */
    uint16_t GetMemoryAddress();
    
    /**
     * Loads a memory address into a 16 bit register
     * 
     * \param highReg the upper 8 bits of the register.
     * \param lowReg the lower 8 bits of the register.
     * \return the memory address stored into the register.
     */
    uint16_t LoadMemoryReg(int8_t *highReg, int8_t *lowReg);
    
    /**
     * Get a 16-bit value from a regiter.
     *
     * \param highReg the upper 8 bits of the register.
     * \param lowReg the lower 8 bits of the register.
     * \return 16-bit value stored in the register.
     */
    int16_t Get16BitRegister(int8_t highReg, int8_t lowReg);
    
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
    void SetImmValue(uint16_t value);

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
    z80Reg registers;
    /* Test Flags */
    z80Flags flags;
};

#endif
