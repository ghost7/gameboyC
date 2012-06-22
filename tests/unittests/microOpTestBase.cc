#include "microOpTestBase.h"

MemoryEmulator::MemoryEmulator()
{
    clearMemory();
}

uint8_t MemoryEmulator::memoryRead(uint16_t memAddr)
{
    return memory[memAddr];
}

void MemoryEmulator::memoryWrite(uint16_t memAddr, uint8_t data)
{
    memory[memAddr] = data;
}

void MemoryEmulator::clearMemory()
{
    memset(memory, 0, sizeof(uint8_t) * MEM_SIZE);
}

void MicroOpTestBase::SetUp()
{
    // zero out registers, and memory
    memset(&registers, 0, sizeof(Z80Registers));
    memset(&flags, 0, sizeof(Z80Flags));
    memory.clearMemory();

    instSet = new Z80InstructionSet(&memory, &registers);
    flags = registers.getFlags();
}

uint16_t MicroOpTestBase::GetMemoryAddress()
{
    return rand() % (MEM_SIZE - 0xFF);
}

uint8_t MicroOpTestBase::GetRandomNumber()
{
    return rand() % 0xFF;
}

uint16_t MicroOpTestBase::LoadMemoryReg(RegisterPair *regPair)
{
    uint16_t memoryAddress = GetMemoryAddress();
    regPair->val = memoryAddress;
    return memoryAddress;
}

uint16_t MicroOpTestBase::Get16BitRegister(uint8_t highReg, uint8_t lowReg)
{
    return ((highReg << 8) & 0xFF00) | (lowReg & 0xFF);
}

void MicroOpTestBase::Set16BitRegister(uint8_t *highReg, uint8_t *lowReg, uint16_t value)
{
    *highReg = (value >> 8) & 0xFF;
    *lowReg = value & 0xFF;
}

void MicroOpTestBase::SetImmValue(uint8_t value)
{
    uint16_t memoryAddress = GetMemoryAddress();
    memory.memoryWrite(memoryAddress, value);
    registers.PC = memoryAddress;
}

void MicroOpTestBase::SetImm16Value(uint16_t value)
{
    uint16_t memoryAddress = GetMemoryAddress();
    memory.memoryWrite(memoryAddress, value & 0xFF);
    memory.memoryWrite(memoryAddress + 1, (value >> 8) & 0xFF);
    registers.PC.val = memoryAddress;
}

void MicroOpTestBase::SetMemory(uint16_t memoryAddress, uint8_t data)
{
    memory.memoryWrite(memoryAddress, data);
}

uint8_t MicroOpTestBase::GetMemory(uint16_t memoryAddress)
{
    return memory.memoryRead(memoryAddress);
}
