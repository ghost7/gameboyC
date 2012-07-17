#include "../Common/Config.h"
#include "Z80Cpu.h"

#define TMA_ADDR 0xFF06
#define IF_ADDR 0xFF0F
#define IE_ADDR 0xFFFF
#define TIMER_REQUEST 0x40

struct HalfRegisters
{
    uint8_t B, C, D, E, H, L, A;
    HalfRegisters(Z80Registers registers)
    {
        B = registers.BC.hi;
        C = registers.BC.lo;
        D = registers.DE.hi;
        E = registers.DE.lo;
        H = registers.HL.hi;
        L = registers.HL.lo;
        A = registers.AF.hi;
    }
    Z80Registers getRegisters()
    {
        Z80Registers registers;
        registers.BC.hi = B;
        registers.BC.lo = C;
        registers.DE.hi = D;
        registers.DE.lo = E;
        registers.HL.hi = H;
        registers.HL.lo = L;
        registers.AF.hi = A;
        return registers;
    }
};

Z80Cpu::Z80Cpu(Memory* mem) 
{
    memory = mem;
    ioMemory = memory->getIOMemory();
    flags = registers.getFlags();
};


void Z80Cpu::init()
{
    instSet = new Z80InstructionSet(memory, &registers);
    // Normally the register values are set by the boot strap program, 
    // but if there is no boot strap, set them here.
    if (!Config::DmgEnabled)
    {
        registers.AF = 0x01B0;
        registers.BC = 0x0013;
        registers.DE = 0x00D8;
        registers.HL = 0x014D;
        registers.PC = 0x100;
    }
}

void Z80Cpu::advanceTimer(int stepTime)
{
    // Get the timer from memory.
    int timer = ioMemory->TIMA; 
    // advance the timer.
    timer += stepTime;

    // Check for overflow.
    if (timer > 0xFF)
    {
        timer = memory->read(TMA_ADDR);
        
        // request the stepTime interrupt.
        data_t intFlags = memory->read(IF_ADDR);
        intFlags |= TIMER_REQUEST;
        memory->write(IF_ADDR, TIMER_REQUEST);
    }

    // write the stepTime back into memory.
    ioMemory->TIMA = timer;
}

int Z80Cpu::checkForInterrupts()
{
    data_t register_IE = memory->read(IE_ADDR);
    data_t register_IF = memory->read(IF_ADDR);
    
    uint8_t interruptState = register_IE & register_IF;
    int stepTime = 0;

    if ((interruptState != 0) && intMasterEnable)
    {
        switch (interruptState)
        {
            case 0x01:                 // Vertical Blank
                stepTime += instSet->sysCall(0x40);
                register_IF = 0;
                break;
            case 0x02:                 // LCD status triggers
                stepTime += instSet->sysCall(0x48);
                register_IF = 0;
                break;
            case 0x04:                 // stepTime overflow
                stepTime += instSet->sysCall(0x50);
                register_IF = 0;
                break;
            case 0x08:                 // Serial link
                stepTime += instSet->sysCall(0x58);
                register_IF = 0;
                break;
            case 0x10:                 // Joypad State
                stepTime += instSet->sysCall(0x60);
                register_IF = 0;
                break;
        }
        memory->write(IF_ADDR, register_IF);
        ioMemory->TIMA += stepTime;
        // disable any futher interrupts until the program 
        // re-enables them.
        intMasterEnable = false;
    }

    return stepTime;
}

int Z80Cpu::step()
{
    int stepTime = 0; 
    // stepTime += checkForInterrupts();
    // Fetch the next instruction
    data_t cpuInst = memory->read(registers.PC.val++);
    // Execute the instruction
    stepTime += executeInstruction(cpuInst);
    
    advanceTimer(stepTime);
    return stepTime;
}

int Z80Cpu::executeInstruction(data_t cpuInst)
{
    int stepTime = 0;
    HalfRegisters hRegisters(registers);
    switch(cpuInst)
    {
        case 0x00: // NOP
            stepTime += 4;
            break;
        case 0x01: // LD BC, NN
            stepTime += instSet->loadReg16(&registers.BC);    
            break;
        case 0x02: // LD (BC), A
            stepTime += instSet->storeA(registers.BC);
            break;
        case 0x03: // INC BC
            stepTime += instSet->incReg16(&registers.BC);
            break;
        case 0x04: // INC B
            stepTime += instSet->incReg8(&hRegisters.B);
            registers = hRegisters.getRegisters();
            break;
        case 0x05: // DEC B
            stepTime += instSet->decReg8(&hRegisters.B);
            registers = hRegisters.getRegisters();
            break;
        case 0x06: // LD B, n
            stepTime += instSet->loadImmReg8(&hRegisters.B);
            registers = hRegisters.getRegisters();
            break;
        case 0x07: // RLCA
            stepTime += instSet->rotateALeftC(); 
            break;
        case 0x08: // LD (nn), SP
            stepTime += instSet-> loadSPImm();
            break;
        case 0x09: // ADD HL, BC
            stepTime += instSet->addReg16(registers.BC);
            break;
        case 0x0A: // LD A, (BC)
            stepTime += instSet->loadA(registers.BC);
            break;
        case 0x0B: // DEC BC
            stepTime += instSet->decReg16(&registers.BC);
            break;
        case 0x0C: // INC C
            stepTime += instSet->incReg8(&hRegisters.C);
            registers = hRegisters.getRegisters();
            break;
        case 0x0D: // DEC C
            stepTime += instSet->decReg8(&hRegisters.C);
            registers = hRegisters.getRegisters();
            break;
        case 0x0E: // LD C, n
            stepTime += instSet->loadImmReg8(&hRegisters.C);
            registers = hRegisters.getRegisters(); 
            break;
        case 0x0F: // RRCA
            stepTime += instSet->rotateARightC();
            break;
        case 0x10: // STOP
            stepTime += instSet->stop(); 
            break;
        case 0x11: // LD DE, nn
            stepTime += instSet->loadReg16(&registers.DE); 
            break;
        case 0x12: // LD (DE), A
            stepTime += instSet->storeA(registers.DE); 
            break;
        case 0x13: // INC DE
            stepTime += instSet->incReg16(&registers.DE);
            break;
        case 0x14: // INC D
            stepTime += instSet->incReg8(&hRegisters.D);
            registers = hRegisters.getRegisters();
            break;
        case 0x15: // DEC D
            stepTime += instSet->decReg8(&hRegisters.D);
            registers = hRegisters.getRegisters();
            break;
        case 0x16: // LD D, n
            stepTime += instSet->loadImmReg8(&hRegisters.D);
            registers = hRegisters.getRegisters();
            break;
        case 0x17: // RLA
            stepTime += instSet->rotateALeft(); 
            break;
        case 0x18: // JR d
            stepTime += instSet->relativeJump();
            break;
        case 0x19: // ADD HL, DE
            stepTime += instSet->addReg16(registers.DE);
            break;
        case 0x1A: // LD A, (DE)
            stepTime += instSet->loadA(registers.DE);
            break;
        case 0x1B: // DEC DE
            stepTime += instSet->decReg16(&registers.DE);
            break;
        case 0x1C: // INC E
            stepTime += instSet->incReg8(&hRegisters.E);
            registers = hRegisters.getRegisters();
            break;
        case 0x1D: // DEC E 
            stepTime += instSet->decReg8(&hRegisters.E);
            registers = hRegisters.getRegisters();
            break;
        case 0x1E: // LD E, n
            stepTime += instSet->loadImmReg8(&hRegisters.E);
            registers = hRegisters.getRegisters();
            break;
        case 0x1F: // RRA
            stepTime += instSet->rotateARight();
            break;
        case 0x20: // JR NZ, d 
            stepTime += instSet->conditionalRelativeJump(flags->Z == 0);  
            break;
        case 0x21: // LD HL, nn
            stepTime += instSet->loadReg16(&registers.HL);
            break;
        case 0x22: // LDI (HL), A
            stepTime += instSet->storeIncrement();  
            break;
        case 0x23: // INC HL
            stepTime += instSet->incReg16(&registers.HL);
            break;
        case 0x24: // INC H
            stepTime += instSet->incReg8(&hRegisters.H);
            registers = hRegisters.getRegisters();
            break;
        case 0x25: // DEC H
            stepTime += instSet->decReg8(&hRegisters.H);
            registers = hRegisters.getRegisters();
            break;
        case 0x26: // LD H, n
            stepTime += instSet->loadImmReg8(&hRegisters.H);
            registers = hRegisters.getRegisters();
            break;
        case 0x27: // DAA
            stepTime += instSet->decimalyAdjustA();
            break;
        case 0x28: // JR Z, d
            stepTime += instSet->conditionalRelativeJump(flags->Z == 1); 
            break;
        case 0x29: // ADD HL, HL
            stepTime += instSet->addReg16(registers.HL);
            break;
        case 0x2A: // LDI A, (HL)
            stepTime += instSet->loadIncrement();
            break;
        case 0x2B: // DEC HL
            stepTime += instSet->decReg16(&registers.HL);
            break;
        case 0x2C: // INC L
            stepTime += instSet->incReg8(&hRegisters.L);
            registers = hRegisters.getRegisters(); 
            break;
        case 0x2D: // DEC L
            stepTime += instSet->decReg8(&hRegisters.L);
            registers = hRegisters.getRegisters();
            break;
        case 0x2E: // LD L, n
            stepTime += instSet->loadImmReg8(&hRegisters.L);
            registers = hRegisters.getRegisters();
            break;
        case 0x2F: // CPL
            stepTime += instSet->complementA();
            break;
        case 0x30: // JR NC, d
            stepTime += instSet->conditionalRelativeJump(flags->C == 0);
            break;
        case 0x31: // LD SP, nn
            stepTime += instSet->loadSPImm();
            break;
        case 0x32: // LDD (HL), A
            stepTime += instSet->storeDecrement();
            break;
        case 0x33: // INC SP
            registers.SP.val++;
            stepTime += 8;
            break;
        case 0x34: // INC (HL)
            stepTime += instSet->incHLInd();
            break;
        case 0x35: // DEC (HL)
            stepTime += instSet->decHLInd(); 
            break;
        case 0x36: // LD (HL), n
            stepTime += instSet->storeHLImm();
            break;
        case 0x37: // SCF
            stepTime += instSet->setCarry(); 
            break;
        case 0x38: // JR C, d
            stepTime += instSet->conditionalRelativeJump(flags->C == 0);
            break;
        case 0x39: // ADD HL, SP
            stepTime += instSet->addReg16(registers.SP.val); 
            break;
        case 0x3A: // LDD A, (HL)
            stepTime += instSet->loadDecrement(); 
            break;
        case 0x3B: // DEC SP
            registers.SP.val--;
            stepTime += 8;
            break; 
        case 0x3C: // INC A
            stepTime += instSet->incReg8(&hRegisters.A);
            registers = hRegisters.getRegisters(); 
            break;
        case 0x3D: // DEC A
            stepTime += instSet->decReg8(&hRegisters.A);
            registers = hRegisters.getRegisters();
            break;
        case 0x3E: // LD A, n
            stepTime += instSet->loadImmReg8(&hRegisters.A);
            registers = hRegisters.getRegisters();
            break;
        case 0x3F: // CCF
            stepTime += instSet->complementCarry();
            break;
        case 0x40: // LD B, B
            stepTime += instSet->loadReg8(&hRegisters.B, &hRegisters.B);
            registers = hRegisters.getRegisters();
            break;
        case 0x41: // LD B, C
            stepTime += instSet->loadReg8(&hRegisters.C, &hRegisters.B);
            registers = hRegisters.getRegisters();
            break;
        case 0x42: // LD B, D
            stepTime += instSet->loadReg8(&hRegisters.D, &hRegisters.B);
            registers = hRegisters.getRegisters();
            break;
        case 0x43: // LD B, E
            stepTime += instSet->loadReg8(&hRegisters.E, &hRegisters.B);
            registers = hRegisters.getRegisters();
            break;
        case 0x44: // LD B, H
            stepTime += instSet->loadReg8(&hRegisters.H, &hRegisters.B);
            registers = hRegisters.getRegisters();
            break;
        case 0x45: // LD B, L
            stepTime += instSet->loadReg8(&hRegisters.L, &hRegisters.B);
            registers = hRegisters.getRegisters();
            break;
        case 0x46: // LD B, (HL)
            stepTime += instSet->loadReg8HL(&hRegisters.B);
            registers = hRegisters.getRegisters(); 
            break;
        case 0x47: // LD B, A
            stepTime += instSet->loadReg8(&hRegisters.A, &hRegisters.B);
            registers = hRegisters.getRegisters();
            break;
        case 0x48: // LD C, B
            stepTime += instSet->loadReg8(&hRegisters.B, &hRegisters.C);
            registers = hRegisters.getRegisters();
            break;
        case 0x49: // LD C, C
            stepTime += instSet->loadReg8(&hRegisters.C, &hRegisters.C);
            registers = hRegisters.getRegisters();
            break;
        case 0x4A: // LD C, D
            stepTime += instSet->loadReg8( &hRegisters.D, &hRegisters.C);
            registers = hRegisters.getRegisters();
            break;
        case 0x4B:// LD C, E
            stepTime += instSet->loadReg8(&hRegisters.E, &hRegisters.C);
            registers = hRegisters.getRegisters();
            break;
        case 0x4C: // LD C, H
            stepTime += instSet->loadReg8(&hRegisters.H, &hRegisters.C);
            registers = hRegisters.getRegisters();
            break;
        case 0x4D: // LD C, L
            stepTime += instSet->loadReg8(&hRegisters.L, &hRegisters.C);
            registers = hRegisters.getRegisters();
            break;
        case 0x4E: // LD C, (HL)
            stepTime += instSet->loadReg8HL(&hRegisters.C);
            registers = hRegisters.getRegisters(); 
            break;
        case 0x4F: // LD C, A
            stepTime += instSet->loadReg8(&hRegisters.A, &hRegisters.C);
            registers = hRegisters.getRegisters();
            break;
        case 0x50: // LD D, B
            stepTime += instSet->loadReg8(&hRegisters.B, &hRegisters.D);
            registers = hRegisters.getRegisters();
            break;
        case 0x51: // LD D, C
            stepTime += instSet->loadReg8(&hRegisters.C, &hRegisters.D);
            registers = hRegisters.getRegisters();
            break;
        case 0x52: // LD D, D
            stepTime += instSet->loadReg8(&hRegisters.D, &hRegisters.D);
            registers = hRegisters.getRegisters();
            break;
        case 0x53: // LD D, E
            stepTime += instSet->loadReg8(&hRegisters.E, &hRegisters.D);
            registers = hRegisters.getRegisters();
            break;
        case 0x54: // LD D, H
            stepTime += instSet->loadReg8(&hRegisters.H, &hRegisters.D);
            registers = hRegisters.getRegisters();
            break;
        case 0x55: // LD D, L
            stepTime += instSet->loadReg8(&hRegisters.L, &hRegisters.D);
            registers = hRegisters.getRegisters();
            break;
        case 0x56: // LD D, (HL)
            stepTime += instSet->loadReg8HL(&hRegisters.D); 
            registers = hRegisters.getRegisters();
            break;
        case 0x57: // LD D, A
            stepTime += instSet->loadReg8(&hRegisters.A, &hRegisters.D);
            registers = hRegisters.getRegisters();
            break;
        case 0x58: // LD E, B
            stepTime += instSet->loadReg8(&hRegisters.B, &hRegisters.E);
            registers = hRegisters.getRegisters();
            break;
        case 0x59: // LD E, C
            stepTime += instSet->loadReg8(&hRegisters.C, &hRegisters.E);
            registers = hRegisters.getRegisters();
            break;
        case 0x5A: // LD E, D
            stepTime += instSet->loadReg8(&hRegisters.D, &hRegisters.E);
            registers = hRegisters.getRegisters();
            break;
        case 0x5B: // LD E, E
            stepTime += instSet->loadReg8(&hRegisters.E, &hRegisters.E);
            registers = hRegisters.getRegisters();
            break;
        case 0x5C: // LD E, H
            stepTime += instSet->loadReg8(&hRegisters.H, &hRegisters.E);
            registers = hRegisters.getRegisters();
            break;
        case 0x5D: // LD E, L
            stepTime += instSet->loadReg8(&hRegisters.L, &hRegisters.E);
            registers = hRegisters.getRegisters();
            break;
        case 0x5E: // LD E, (HL)
            stepTime += instSet->loadReg8HL(&hRegisters.E); 
            registers = hRegisters.getRegisters();
            break;
        case 0x5F: // LD E, A
            stepTime += instSet->loadReg8( &hRegisters.B, &hRegisters.E);
            registers = hRegisters.getRegisters();
            break;
        case 0x60: // LD H, B
            stepTime += instSet->loadReg8(&hRegisters.B, &hRegisters.H);
            registers = hRegisters.getRegisters();
            break;
        case 0x61: // LD H, C
            stepTime += instSet->loadReg8(&hRegisters.C, &hRegisters.H);
            registers = hRegisters.getRegisters();
            break;
        case 0x62: // LD H, D
            stepTime += instSet->loadReg8(&hRegisters.D, &hRegisters.H);
            registers = hRegisters.getRegisters();
            break;
        case 0x63: // LD H, E
            stepTime += instSet->loadReg8(&hRegisters.E, &hRegisters.H);
            registers = hRegisters.getRegisters();
            break;
        case 0x64: // LD H, H
            stepTime += instSet->loadReg8(&hRegisters.H, &hRegisters.H);
            registers = hRegisters.getRegisters();
            break;
        case 0x65: // LD H, L
            stepTime += instSet->loadReg8(&hRegisters.L, &hRegisters.H);
            registers = hRegisters.getRegisters();
            break;
        case 0x66: // LD H, (HL)
            stepTime += instSet->loadReg8HL(&hRegisters.H); 
            registers = hRegisters.getRegisters();
            break;
        case 0x67: // LD H, A
            stepTime += instSet->loadReg8(&hRegisters.A, &hRegisters.H);
            registers = hRegisters.getRegisters();
            break;
        case 0x68: // LD L, B
            stepTime += instSet->loadReg8(&hRegisters.B, &hRegisters.L);
            registers = hRegisters.getRegisters();
            break;
        case 0x69: // LD L, C
            stepTime += instSet->loadReg8(&hRegisters.C, &hRegisters.L);
            registers = hRegisters.getRegisters();
            break;
        case 0x6A: // LD L, D
            stepTime += instSet->loadReg8(&hRegisters.D, &hRegisters.L);
            registers = hRegisters.getRegisters();
            break;
        case 0x6B: // LD L, E
            stepTime += instSet->loadReg8(&hRegisters.E, &hRegisters.L);
            registers = hRegisters.getRegisters();
            break;
        case 0x6C: // LD L, H
            stepTime += instSet->loadReg8(&hRegisters.H, &hRegisters.L);
            registers = hRegisters.getRegisters();
            break;
        case 0x6D: // LD L, L
            stepTime += instSet->loadReg8(&hRegisters.L, &hRegisters.L);
            registers = hRegisters.getRegisters();
            break;
        case 0x6E: // LD L, (HL)
            stepTime += instSet->loadReg8HL(&hRegisters.L); 
            registers = hRegisters.getRegisters();
            break;
        case 0x6F: // LD L, A
            stepTime += instSet->loadReg8(&hRegisters.A, &hRegisters.L);
            registers = hRegisters.getRegisters();
            break;
        case 0x70: // LD (HL), B
            stepTime += instSet->storeReg8HL(hRegisters.B); 
            break;
        case 0x71: // LD (HL), C
            stepTime += instSet->storeReg8HL(hRegisters.C);
            break;
        case 0x72: // LD (HL), D
            stepTime += instSet->storeReg8HL(hRegisters.D);
            break;
        case 0x73: // LD (HL), E
            stepTime += instSet->storeReg8HL(hRegisters.E);
            break;
        case 0x74: // LD (HL), H
            stepTime += instSet->storeReg8HL(hRegisters.H);
            break;
        case 0x75: // LD (HL), L
            stepTime += instSet->storeReg8HL(hRegisters.L);
            break;
        case 0x76: // HALT
            stepTime += instSet->halt(); 
            break;
        case 0x77: // LD (HL), A
            stepTime += instSet->storeReg8HL(hRegisters.A);
            break;
        case 0x78: // LD A, B
            stepTime += instSet->loadReg8(&hRegisters.B, &hRegisters.A);
            registers = hRegisters.getRegisters();
            break;
        case 0x79: // LD A, C
            stepTime += instSet->loadReg8(&hRegisters.C, &hRegisters.A);
            registers = hRegisters.getRegisters();
            break;
        case 0x7A: // LD A, D
            stepTime += instSet->loadReg8(&hRegisters.D, &hRegisters.A);
            registers = hRegisters.getRegisters();
            break;
        case 0x7B: // LD A, E
            stepTime += instSet->loadReg8(&hRegisters.E, &hRegisters.A);
            registers = hRegisters.getRegisters();
            break;
        case 0x7C: // LD A, H
            stepTime += instSet->loadReg8(&hRegisters.H, &hRegisters.A);
            registers = hRegisters.getRegisters();
            break;
        case 0x7D: // LD A, L
            stepTime += instSet->loadReg8(&hRegisters.L, &hRegisters.A);
            registers = hRegisters.getRegisters();
            break;
        case 0x7E: // LD A, (HL)
            stepTime += instSet->loadReg8HL(&hRegisters.A); 
            registers = hRegisters.getRegisters();
            break;
        case 0x7F: // LD A, A
            stepTime += instSet->loadReg8(&hRegisters.A, &hRegisters.A);
            registers = hRegisters.getRegisters();
                break;
        case 0x80: // ADD A, B
            stepTime += instSet->addReg(hRegisters.B); 
            break;
        case 0x81: // ADD A, C
            stepTime += instSet->addReg(hRegisters.C);
            break;
        case 0x82: // ADD A, D
            stepTime += instSet->addReg(hRegisters.D);
            break;
        case 0x83: // ADD A, E
            stepTime += instSet->addReg(hRegisters.E);
            break;
        case 0x84: // ADD A, H
            stepTime += instSet->addReg(hRegisters.H);
            break;
        case 0x85: // ADD A, L
            stepTime += instSet->addReg(hRegisters.L);
            break;
        case 0x86: // ADD A, (HL)
            stepTime += instSet->addHLInd(); 
            break;
        case 0x87: // ADD A, A
            stepTime += instSet->addReg(hRegisters.A);
            break;
        case 0x88: // ADC A, B
            stepTime += instSet->adcReg(hRegisters.B); 
            break;
        case 0x89: // ADC A, C
            stepTime += instSet->adcReg(hRegisters.C);
            break;
        case 0x8A: // ADC A, D
            stepTime += instSet->adcReg(hRegisters.D);
            break;
        case 0x8B: // ADC A, E
            stepTime += instSet->adcReg(hRegisters.E);
            break;
        case 0x8C: // ADC A, H
            stepTime += instSet->adcReg(hRegisters.H);
            break;
        case 0x8D: // ADC A, L
            stepTime += instSet->adcReg(registers.HL.lo);
            break;
        case 0x8E: // ADC A, (HL)
            stepTime += instSet->adcHLInd(); 
            break;
        case 0x8F: // ADC A, A
            stepTime += instSet->adcReg(hRegisters.A);
            break;
        case 0x90: // SUB B
            stepTime += instSet->subReg(hRegisters.B);
            break;
        case 0x91: // SUB C
            stepTime += instSet->subReg(hRegisters.C);
            break;
        case 0x92: // SUB D
            stepTime += instSet->subReg(hRegisters.D);
            break;
        case 0x93: // SUB E
            stepTime += instSet->subReg(hRegisters.E);
            break;
        case 0x94: // SUB H
            stepTime += instSet->subReg(hRegisters.H);
            break;
        case 0x95: // SUB L
            stepTime += instSet->subReg(hRegisters.L);
            break;
        case 0x96: // SUB (HL)
            stepTime += instSet->subHLInd(); 
            break;
        case 0x97: // SUB A
            stepTime += instSet->subReg(hRegisters.A);
            break;
        case 0x98: // SBC A, B
            stepTime += instSet->sbcReg(hRegisters.B);
            break;
        case 0x99: // SBC A, C
            stepTime += instSet->sbcReg(hRegisters.C);
            break;
        case 0x9A: // SBC A, D
            stepTime += instSet->sbcReg(hRegisters.D);
            break;
        case 0x9B: // SBC A, E
            stepTime += instSet->sbcReg(hRegisters.E);
            break;
        case 0x9C: // SBC A, H
            stepTime += instSet->sbcReg(hRegisters.H);
            break;
        case 0x9D: // SBC A, L
            stepTime += instSet->sbcReg(hRegisters.L);
            break;
        case 0x9E: // SBC A, (HL)
            stepTime += instSet->subHLInd(); 
            break;
        case 0x9F: // SBC A, A
            stepTime += instSet->sbcReg(hRegisters.A);
            break;
        case 0xA0: // AND B
            stepTime += instSet->andReg(hRegisters.B); 
            break;
        case 0xA1: // AND C
            stepTime += instSet->andReg(hRegisters.C);
            break;
        case 0xA2: // AND D
            stepTime += instSet->andReg(hRegisters.D);
            break;
        case 0xA3: // AND E
            stepTime += instSet->andReg(hRegisters.E);
            break;
        case 0xA4: // AND H
            stepTime += instSet->andReg(hRegisters.H);
            break;
        case 0xA5: // AND L
            stepTime += instSet->andReg(hRegisters.L);
            break;
        case 0xA6: // AND (HL)
            stepTime += instSet->andHLInd(); 
            break;
        case 0xA7: // AND A
            stepTime += instSet->andReg(hRegisters.A);
            break;
        case 0xA8: // XOR B 
            stepTime += instSet->xorReg(hRegisters.B);
            break;
        case 0xA9: // XOR C
            stepTime += instSet->xorReg(hRegisters.C);
            break;
        case 0xAA: // XOR D
            stepTime += instSet->xorReg(hRegisters.D);
            break;
        case 0xAB: // XOR E
            stepTime += instSet->xorReg(hRegisters.E);
            break;
        case 0xAC: // XOR H
            stepTime += instSet->xorReg(hRegisters.H);
            break;
        case 0xAD: // XOR L
            stepTime += instSet->xorReg(hRegisters.L);
            break;
        case 0xAE: // XOR (HL)
            stepTime += instSet->xorHLInd(); 
            break;
        case 0xAF: // XOR A
            stepTime += instSet->xorReg(hRegisters.A);
            break;
        case 0xB0: // OR B
            stepTime += instSet->orReg(hRegisters.B);
            break;
        case 0xB1: // OR C
            stepTime += instSet->orReg(hRegisters.C);
            break;
        case 0xB2: // OR D
            stepTime += instSet->orReg(hRegisters.D);
            break;
        case 0xB3: // OR E
            stepTime += instSet->orReg(hRegisters.E);
            break;
        case 0xB4: // OR H
            stepTime += instSet->orReg(hRegisters.H);
            break;
        case 0xB5: // OR L
            stepTime += instSet->orReg(hRegisters.L);
            break;
        case 0xB6: // OR (HL)
            stepTime += instSet->orHLInd(); 
            break;
        case 0xB7: // OR A
            stepTime += instSet->orReg(hRegisters.A);
            break;
        case 0xB8: // CP B
            stepTime += instSet->compareReg(hRegisters.B);
            break;
        case 0xB9: // CP C
            stepTime += instSet->compareReg(hRegisters.C);
            break;
        case 0xBA: // CP D
            stepTime += instSet->compareReg(hRegisters.D);
            break;
        case 0xBB: // CP E
            stepTime += instSet->compareReg(hRegisters.E);
            break;
        case 0xBC: // CP H
            stepTime += instSet->compareReg(hRegisters.H);
            break;
        case 0xBD: // CP L
            stepTime += instSet->compareReg(hRegisters.L);
            break;
        case 0xBE: // CP (HL)
            stepTime += instSet->compareHLInd(); 
            break;
        case 0xBF: // CP A
            stepTime += instSet->compareReg(hRegisters.A);
            break;
        case 0xC0: // RET NZ
            stepTime += instSet->conditionalReturnPC(flags->Z == 0); 
            break;
        case 0xC1: // POP BC
            stepTime += instSet->popFromStack(&registers.BC);
            break;
        case 0xC2: // JP NZ, nn
            stepTime += instSet->conditionalJump(flags->Z == 0); 
            break;
        case 0xC3: // JP nn
            stepTime += instSet->jump();
            break;
        case 0xC4: // CALL NZ, nn
            stepTime += instSet->conditionalCall(flags->Z == 0); 
            break;
        case 0xC5: // PUSH BC
            stepTime += instSet->pushToStack(registers.BC);
            break;
        case 0xC6: // ADD A, n
            stepTime += instSet->addImm(); 
            break;
        case 0xC7: // RST 0
            stepTime += instSet->sysCall(0x00); 
            break;
        case 0xC8: // RET Z
            stepTime += instSet->conditionalReturnPC(flags->Z == 1);
            break;
        case 0xC9: // RET
            stepTime += instSet->returnPC();
            break;
        case 0xCA: // JP Z, nn
            stepTime += instSet->conditionalJump(flags->Z == 1);
            break;
        case 0xCB: // Prefix
            stepTime += executeCBInstruction();
            break;
        case 0xCC: // CALL Z, nn
            stepTime += instSet->conditionalCall(flags->Z == 1);
            break;
        case 0xCD: // CALL nn
            stepTime += instSet->call(); 
            break;
        case 0xCE: // ADC A, n
            stepTime += instSet->adcImm();
            break;
        case 0xCF: // RST 8
            stepTime += instSet->sysCall(0x08);
            break;
        case 0xD0: // RET NC
            stepTime += instSet->conditionalReturnPC(flags->C == 0);
            break;
        case 0xD1: // POP DE
            stepTime += instSet->popFromStack(&registers.DE);
            break;
        case 0xD2: // JP NC, nn
            stepTime += instSet->conditionalJump(flags->C == 0);
            break;
        case 0xD4: // CALL NC, nn
            stepTime += instSet->conditionalCall(flags->C == 0); 
            break;
        case 0xD5: // PUSH DE
            stepTime += instSet->pushToStack(registers.DE);
            break;
        case 0xD6: // SUB n
            stepTime += instSet->subImm();
            break;
        case 0xD7: // RST 10H
            stepTime += instSet->sysCall(0x10);
            break;
        case 0xD8: // RET C
            stepTime += instSet->conditionalReturnPC(flags->C == 0); 
            break;
        case 0xD9: // RETI
            stepTime += instSet->returnPCI(&intMasterEnable);
            break;
        case 0xDA: // JP C, nn
            stepTime += instSet->conditionalJump(flags->C == 0); 
            break;
        case 0xDC: // CALL C, nn
            stepTime += instSet->conditionalCall(flags->C == 0);
            break;
        case 0xDE: // SBC A, n
            stepTime += instSet->sbcImm(); 
            break;
        case 0xDF: // RST 18H
            stepTime += instSet->sysCall(0x18); 
            break;
        case 0xE0: // LD (FF00+n), A
            stepTime += instSet->writeIOPortN();
            break;
        case 0xE1: // POP HL
            stepTime += instSet->popFromStack(&registers.HL);
            break;
        case 0xE2: // LD (FF00+C), A
            stepTime += instSet->writeIOPortC();
            break;
        case 0xE5: // PUSH HL
            stepTime += instSet->pushToStack(registers.HL); 
            break;
        case 0xE6: // AND n
            stepTime += instSet->andImm();
            break;
        case 0xE7: // RST 20H
            stepTime += instSet->sysCall(0x20);
            break;
        case 0xE8: // ADD SP, dd
            stepTime += instSet->addImmToSP();
            break;
        case 0xE9: // JP (HL)
            stepTime += instSet->jumpHL(); 
            break;
        case 0xEA: // LD (nn), A
            stepTime += instSet->storeAInd(); 
            break;
        case 0xEE: // XOR n
            stepTime += instSet->xorImm(); 
            break;
        case 0xEF: // RST 28H
            stepTime += instSet->sysCall(0x28);
            break;
        case 0xF0: // LD A, (FF00+n)
            stepTime += instSet->readIOPortN();
            break;
        case 0xF1: // POP AF
            stepTime += instSet->popFromStack(&registers.AF);
            break;
        case 0xF2: // LD A, (FF00+C)
            stepTime += instSet->readIOPortC(); 
            break;
        case 0xF3: // DI
            stepTime += instSet->disableInterrupts(&intMasterEnable); 
            break;
        case 0xF5: // PUSH AF
            stepTime += instSet->pushToStack(hRegisters.A);
            break;
        case 0xF6: // OR n
            stepTime += instSet->orImm(); 
            break;
        case 0xF7: // RST 30H
            stepTime += instSet->sysCall(0x30);
            break;
        case 0xF8: // LD HL, SP+dd
            stepTime += instSet->addSPToHL();
            break;
        case 0xF9: // LD SP, HL
            stepTime += instSet->loadHLToSP();
            break;
        case 0xFA: // LD A, (nn)
            stepTime += instSet->loadAInd(); 
            break;
        case 0xFB: // EI
            stepTime += instSet->enableInterrupts(&intMasterEnable);
            break;
        case 0xFE: // CP n
            stepTime += instSet->compareImm();
            break;
        case 0xFF: // RST 38H
            stepTime += instSet->sysCall(0x38);
            break;
    }
    return stepTime;
}

int Z80Cpu::executeCBInstruction()
{
    int stepTime = 0;

    // break the instruction down
    data_t inst = memory->read(registers.PC.val++);
    int highInst = (inst >> 4) & 0xF;
    int lowInst = inst & 0xF;

    // value that is being used
    uint8_t* reg = 0;

    // flag if (HL) is being used
    int isInd = 0;
    uint8_t hlMem = 0;
    int memWrite = 1;

    HalfRegisters hRegisters(registers);
    // find out which value is going to be used
    switch(lowInst)
    {
        case 0x0:
            reg = &hRegisters.B;
            break;
        case 0x1:
            reg = &hRegisters.C;
            break;
        case 0x2:
            reg = &hRegisters.D;
            break;
        case 0x3:
            reg = &hRegisters.E;
            break;
        case 0x4:
            reg = &hRegisters.H;
            break;
        case 0x5:
            reg = &hRegisters.L;
            break;
        case 0x6:
            // (HL) is being used, grab it from memory and set the flag
            isInd = 1;
            hlMem = memory->read(registers.HL.val);
            reg = &hlMem;
            break;
        case 0x7:
            reg = &hRegisters.A;
            break;
        case 0x8:
            reg = &hRegisters.B;
            break;
        case 0x9:
            reg = &hRegisters.C;
            break;
        case 0xA:
            reg = &hRegisters.D;
            break;
        case 0xB:
            reg = &hRegisters.E;
            break;
        case 0xC:
            reg = &hRegisters.H;
            break;
        case 0xD:
            reg = &hRegisters.L;
            break;
        case 0xE:
            // (HL) is being used, grab it from memory and set the flag
            isInd = 1;
            hlMem = memory->read(registers.HL.val);
            reg = &hlMem;
            break;
        case 0xF:
            reg = &hRegisters.A;
            break;
    }
    
    switch(highInst)
    {
        case 0x0:
            if(lowInst < 8) // RLC
                stepTime += instSet->rotateRegLeftC(reg);
            else            // RRC
                stepTime += instSet->rotateRegRightC(reg);
            break;
        case 0x1:
            if(lowInst < 8) // RL
                stepTime += instSet->rotateRegLeft(reg);
            else            // RR
                 stepTime += instSet->rotateRegRight(reg);
            break;
        case 0x2:
            if(lowInst < 8) // SLA
                stepTime += instSet->shiftRegLeftA(reg);
            else            // SRA
                stepTime += instSet->shiftRegRightA(reg);
            break;            
        case 0x3:
            if(lowInst < 8) // SWAP
                stepTime += instSet->swapReg(reg);
            else            // SRL
                stepTime += instSet->shiftRegRightL(reg);
            break;
        case 0x4:           // BIT
        case 0x5:
        case 0x6:
        case 0x7:
            if (lowInst < 8)
                stepTime += instSet->testRegBit(reg, (highInst - 4) * 2);
            else
                stepTime += instSet->testRegBit(reg, ((highInst - 4) * 2) + 1);
            // test bit does not need to write back to memory.
            memWrite = 0;
            break;
        case 0x8:            // RES
        case 0x9:
        case 0xA:
        case 0xB:
            if (lowInst < 8)
                stepTime += instSet->resetRegBit(reg, (highInst - 8) * 2);
            else
                stepTime += instSet->resetRegBit(reg, ((highInst - 8) * 2) + 1);
            break;
        case 0xC:             // SET
        case 0xD:
        case 0xE:
        case 0xF:
            if (lowInst < 8)
                stepTime += instSet->resetRegBit(reg, (highInst - 0xC) * 2);
            else
                stepTime += instSet->resetRegBit(reg, ((highInst - 0xC) * 2) + 1);
            break;
    }
    
    // (HL) was used
    if(isInd)
    {
        // add time needed to read from memory
        stepTime += 4;
        // check if we need to write back to memory.
        if(memWrite)
        {
            memory->write(registers.HL.val, hlMem);
            // add time to write to memory 
            stepTime += 4;
        }
    }
    else
    {
        registers = hRegisters.getRegisters();
    }
    return stepTime;
}
