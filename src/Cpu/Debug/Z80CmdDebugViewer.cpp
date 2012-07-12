#include <iostream>
#include <stdio.h>
#include "Z80CmdDebugViewer.h"

void Z80CmdDebugViewer::show()
{
    debugger->attach();
    debugger->addBreakPoint(registers->PC.val);
}

void Z80CmdDebugViewer::update()
{
    if (debugger->pauseEmulation())
    {
        std::string command;
        std::cout << ": ";
        getline(std::cin, command);
        executeCommand(command);
    }
}

void Z80CmdDebugViewer::executeCommand(std::string command)
{
    if (command.compare("p") == 0)
    {
        printInstruction();
    }
    else if (command.compare("r") == 0)
    {
        printRegisters();
    }
    else if (command.compare("s") == 0)
    {
        debugger->step();
    }
    else if (command.compare("c") == 0)
    {
        debugger->runToNextBreakPoint();
    }
    else if (command.compare("help") == 0)
    {
        printUsage();
    }
    else
    {
        std::cout << "Invalid command, type 'help' for more information" << std::endl;
    }
}

void Z80CmdDebugViewer::printInstruction()
{
    printf("0x%04x - ", registers->PC.val);
    uint8_t opCode = memory->read(registers->PC.val);
    if (opCode == 0xCB)
    {
        uint8_t cbOpCode = memory->read(registers->PC.val + 1);
        printf("0xCB%02X : %s\n", cbOpCode, instMap.getCBInstruction(cbOpCode));
    }
    else
    {
        printf("0x%02X : %s\n", opCode, instMap.getInstruction(opCode));
    }
}

void Z80CmdDebugViewer::printRegisters()
{
    printf("{ AF: 0x%04X, BC: 0x%04X, DE: 0x%04X, HL: 0x%04X, PC: 0x%04X, SP: 0x%04X }\n",
        registers->AF,
        registers->BC,
        registers->DE,
        registers->HL,
        registers->PC,
        registers->SP);
}

void Z80CmdDebugViewer::printUsage()
{
    std::cout << "Z80 Command Line Debugger:" << std::endl
              << "    p - print instructions" << std::endl;
}