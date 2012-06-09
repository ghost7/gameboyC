#include "microOpTestBase.h"

/* 16-bit addressable memory */
#define MEM_SIZE 0xFFFF

/* This is the test memory */
static uint8_t memory[MEM_SIZE];

/**
 * Reads from the test memory. Used to initialize micro ops.
 */
static uint8_t MemoryRead(uint16_t address)
{
    return memory[address];
}

/**
 * Writes to the test memory. Used to initialize micro ops.
 */
static void MemoryWrite(uint16_t address, uint8_t data)
{
    memory[address] = data;
}

void MicroOpTestBase::SetUp()
{
    // zero out registers, flags, and memory
    memset(&registers, 0, sizeof(z80Reg));
    memset(&flags, 0, sizeof(z80Flags));
    memset(memory, 0, MEM_SIZE);

    initialize_micro_ops(&registers, &flags, MemoryRead, MemoryWrite);
}

uint16_t MicroOpTestBase::GetMemoryAddress()
{
    return rand() % (MEM_SIZE - 0xFF);
}

uint16_t MicroOpTestBase::LoadMemoryReg(int8_t *highReg, int8_t *lowReg)
{
    uint16_t memoryAddress = GetMemoryAddress();
    *highReg = (memoryAddress >> 8) & 0xFF;
    *lowReg = (memoryAddress) & 0xFF;
    return memoryAddress;
}

int16_t MicroOpTestBase::Get16BitRegister(int8_t highReg, int8_t lowReg)
{
    return ((highReg << 8) & 0xFF00) | (lowReg & 0xFF);
}

void MicroOpTestBase::SetImmValue(uint8_t value)
{
    uint16_t memoryAddress = GetMemoryAddress();
    memory[memoryAddress] = value;
    registers.PC = memoryAddress;
}

void MicroOpTestBase::SetImm16Value(uint16_t value)
{
    uint16_t memoryAddress = GetMemoryAddress();
    memory[memoryAddress] = value & 0xFF;
    memory[memoryAddress + 1] = (value >> 8) & 0xFF;
    registers.PC = memoryAddress;
}

void MicroOpTestBase::SetMemory(uint16_t memoryAddress, uint8_t data)
{
    memory[memoryAddress] = data;
}

uint8_t MicroOpTestBase::GetMemory(uint16_t memoryAddress)
{
    return memory[memoryAddress];
}
