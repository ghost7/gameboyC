#ifndef _Z80_CMD_DEBUG_VIEWER_H_
#define _Z80_CMD_DEBUG_VIEWER_H_

#include <string>
#include "../../Common/DebugViewer.h"
#include "../../Memory/Memory.h"
#include "../Z80.h"
#include "Z80Debugger.h"
#include "Z80InstructionMap.h"

class Z80CmdDebugViewer : public DebugViewer
{
public:
	Z80CmdDebugViewer(Z80Registers *z80Registers, MemoryInterface *z80Memory)
    {
		memory = z80Memory;
		registers = z80Registers;
        debugger = new Z80Debugger(registers);
    }
    ~Z80CmdDebugViewer()
    {
        delete debugger;
    }
    void update();
    void show();
private:
    void executeCommand(std::string command);
    void printInstruction();
    void printRegisters();
    void printUsage();
    Z80InstructionMap instMap;
	MemoryInterface *memory;
    Z80Registers *registers;
    Z80Debugger *debugger;
};

#endif
