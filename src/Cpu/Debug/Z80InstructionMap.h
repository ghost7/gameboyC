#ifndef Z80_INSTRUCTION_MAP_H_
#define Z80_INSTRUCTION_MAP_H_

#include <string>
#include <stdint.h>

class Z80InstructionMap
{
public:
    Z80InstructionMap();
    std::string getInstruction(uint8_t opCode);
    std::string getCBInstruction(uint8_t opCode);
private:
    void populateInstructionMap();
    void populateCBInstructionMap();
    std::string instructions[256];
    std::string cbInstructions[256];
};

#endif
