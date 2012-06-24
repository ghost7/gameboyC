#include "Z80Cpu.h"

#define TIMA_ADDR 0XFF05
#define TMA_ADDR 0xFF06
#define IF_ADDR 0xFF0F
#define IE_ADDR 0xFFFF
#define TIMER_REQUEST 0x40

void Z80Cpu::init()
{
    instSet = new Z80InstructionSet(memory, &registers);
    // Normally the register values are set by the boot strap program, 
    // but if there is no boot strap, set them here.
    if (!useBootStrap)
    {
        registers.AF = 0x01B0;
        registers.BC = 0x0013;
        registers.DE = 0x00D8;
        registers.HL = 0x014D;
    }
}

void Z80Cpu::readTimer()
{
    // timer located in TIMA register.
    timer = memory->read(TIMA_ADDR);
}

void Z80Cpu::writeTimer()
{
    if (timer > 0xFF)
    {
        timer = memory->read(TMA_ADDR);
        
        // request the timer interrupt.
        data_t intFlags = memory->read(IF_ADDR);
        intFlags |= TIMER_REQUEST;
        memory->write(IF_ADDR, TIMER_REQUEST);
    }

    // write the timer back into memory.
    memory->write(TIMA_ADDR, timer);
}

void Z80Cpu::checkForInterrupts()
{
    data_t register_IE = memory->read(IE_ADDR);
    data_t register_IF = memory->read(IF_ADDR);
    
    uint8_t interruptState = register_IE & register_IF;

    if ((interruptState != 0) && intMasterEnable)
    {
        switch (interruptState)
        {
            case 0x01:                 // Vertical Blank
                timer += instSet->sysCall(0x40);
                register_IF = 0;
                break;
            case 0x02:                 // LCD status triggers
                timer += instSet->sysCall(0x48);
                register_IF = 0;
                break;
            case 0x04:                 // Timer overflow
                timer += instSet->sysCall(0x50);
                register_IF = 0;
                break;
            case 0x08:                 // Serial link
                timer += instSet->sysCall(0x58);
                register_IF = 0;
                break;
            case 0x10:                 // Joypad State
                timer += instSet->sysCall(0x60);
                register_IF = 0;
                break;
        }
        memory->write(IF_ADDR, register_IF);
        memory->write(TIMA_ADDR, timer);
        // disable any futher interrupts until the program 
        // re-enables them.
        intMasterEnable = false;
    }
}

void Z80Cpu::step()
{
    // Get the timer from memory.
    readTimer();
    checkForInterrupts();
    // Fetch the next instruction
    data_t cpuInst = memory->read(registers.PC.val);
    // Execute the instruction
    executeInstruction(cpuInst);
    // Write the timer back to memory
    writeTimer();
}

void Z80Cpu::executeInstruction(data_t cpuInst)
{
    switch(cpuInst)
    {
        case 0x00: // NOP
            timer += 4;
            break;
        case 0x01: // LD BC, NN
            timer += instSet->loadReg16(&registers.BC);    
            break;
        case 0x02: // LD (BC), A
            timer += instSet->storeA(registers.BC);    
            break;
        case 0x03: // INC BC
            timer += instSet->incReg16(&registers.BC);
            break;
        case 0x04: // INC B
            timer += instSet->incReg8(&registers.BC.hi);
            break;
        case 0x05: // DEC B
            timer += instSet->decReg8(&registers.BC.hi);
            break;
        case 0x06: // LD B, n
            timer += instSet->loadImmReg8(&registers.BC.hi);
            break;
        case 0x07: // RLCA
            timer += instSet->rotateALeftC(); 
            break;
        case 0x08: // LD (nn), SP
            timer += instSet-> loadSPImm();
            break;
        case 0x09: // ADD HL, BC
            timer += instSet->addReg16(registers.BC);
            break;
        case 0x0A: // LD A, (BC)
            timer += instSet->loadA(registers.BC);
            break;
        case 0x0B: // DEC BC
            timer += instSet->decReg16(&registers.BC);
            break;
        case 0x0C: // INC C
            timer += instSet->incReg8(&registers.BC.lo);
            break;
        case 0x0D: // DEC C
            timer += instSet->decReg8(&registers.BC.lo);
            break;
        case 0x0E: // LD C, n
            timer += instSet->loadImmReg8(&registers.BC.lo); 
            break;
        case 0x0F: // RRCA
            timer += instSet->rotateARightC();
            break;
        case 0x10: // STOP
            timer += instSet->stop(); 
            break;
        case 0x11: // LD DE, nn
            timer += instSet->loadReg16(&registers.DE); 
            break;
        case 0x12: // LD (DE), A
            timer += instSet->storeA(registers.DE); 
            break;
        case 0x13: // INC DE
            timer += instSet->incReg16(&registers.DE);
            break;
        case 0x14: // INC D
            timer += instSet->incReg8(&registers.DE.hi);
            break;
        case 0x15: // DEC D
            timer += instSet->decReg8(&registers.DE.hi);
            break;
        case 0x16: // LD D, n
            timer += instSet->loadImmReg8(&registers.DE.hi);
            break;
        case 0x17: // RLA
            timer += instSet->rotateALeft(); 
            break;
        case 0x18: // JR d
            timer += instSet->relativeJump();
            break;
        case 0x19: // ADD HL, DE
            timer += instSet->addReg16(registers.DE);
            break;
        case 0x1A: // LD A, (DE)
            timer += instSet->loadA(registers.DE);
            break;
        case 0x1B: // DEC DE
            timer += instSet->decReg16(&registers.DE);
            break;
        case 0x1C: // INC E
            timer += instSet->incReg8(&registers.DE.lo);
            break;
        case 0x1D: // DEC E 
            timer += instSet->decReg8(&registers.DE.lo);
            break;
        case 0x1E: // LD E, n
            timer += instSet->loadImmReg8(&registers.DE.lo);
            break;
        case 0x1F: // RRA
            timer += instSet->rotateARight();
            break;
        case 0x20: // JR NZ, d 
            timer += instSet->conditionalRelativeJump(flags->Z == 0);  
            break;
        case 0x21: // LD HL, nn
            timer += instSet->loadReg16(&registers.HL);
            break;
        case 0x22: // LDI (HL), A
            timer += instSet->storeIncrement();  
            break;
        case 0x23: // INC HL
            timer += instSet->incReg16(&registers.HL);
            break;
        case 0x24: // INC H
            timer += instSet->incReg8(&registers.HL.hi);
            break;
        case 0x25: // DEC H
            timer += instSet->decReg8(&registers.HL.hi);
            break;
        case 0x26: // LD H, n
            timer += instSet->loadImmReg8(&registers.HL.hi);
            break;
        case 0x27: // DAA
            timer += instSet->decimalyAdjustA();
            break;
        case 0x28: // JR Z, d
            timer += instSet->conditionalRelativeJump(flags->Z == 1); 
            break;
        case 0x29: // ADD HL, HL
            timer += instSet->addReg16(registers.HL);
            break;
        case 0x2A: // LDI A, (HL)
            timer += instSet->loadIncrement();
            break;
        case 0x2B: // DEC HL
            timer += instSet->decReg16(&registers.HL); 
            break;
        case 0x2C: // INC L
            timer += instSet->incReg8(&registers.HL.lo); 
            break;
        case 0x2D: // DEC L
            timer += instSet->decReg8(&registers.HL.lo);
            break;
        case 0x2E: // LD L, n
            timer += instSet->loadImmReg8(&registers.HL.lo);
            break;
        case 0x2F: // CPL
            timer += instSet->complementA();
            break;
        case 0x30: // JR NC, d
            timer += instSet->conditionalRelativeJump(flags->C == 0);
            break;
        case 0x31: // LD SP, nn
            timer += instSet->loadSPImm();
            break;
        case 0x32: // LDD (HL), A
            timer += instSet->storeDecrement();
            break;
        case 0x33: // INC SP
            registers.SP.val++;
            timer += 8;
            break;
        case 0x34: // INC (HL)
            timer += instSet->incHLInd();
            break;
        case 0x35: // DEC (HL)
            timer += instSet->decHLInd(); 
            break;
        case 0x36: // LD (HL), n
            timer += instSet->storeHLImm();
            break;
        case 0x37: // SCF
            timer += instSet->setCarry(); 
            break;
        case 0x38: // JR C, d
            timer += instSet->conditionalRelativeJump(flags->C == 0);
            break;
        case 0x39: // ADD HL, SP
            timer += instSet->addReg16(registers.SP.val); 
            break;
        case 0x3A: // LDD A, (HL)
            timer += instSet->loadDecrement(); 
            break;
        case 0x3B: // DEC SP
            registers.SP.val--;
            timer += 8;
            break; 
        case 0x3C: // INC A
            timer += instSet->incReg8(&registers.AF.hi); 
            break;
        case 0x3D: // DEC A
            timer += instSet->decReg8(&registers.AF.hi);
            break;
        case 0x3E: // LD A, n
            timer += instSet->loadImmReg8(&registers.AF.hi);
            break;
        case 0x3F: // CCF
            timer += instSet->complementCarry();
            break;
        case 0x40: // LD B, B
            timer += instSet->loadReg8(&registers.BC.hi, &registers.BC.hi);
            break;
        case 0x41: // LD B, C
            timer += instSet->loadReg8(&registers.BC.hi, &registers.BC.lo);
            break;
        case 0x42: // LD B, D
            timer += instSet->loadReg8(&registers.BC.hi, &registers.DE.hi);
            break;
        case 0x43: // LD B, E
            timer += instSet->loadReg8(&registers.BC.hi, &registers.DE.lo);
            break;
        case 0x44: // LD B, H
            timer += instSet->loadReg8(&registers.BC.hi, &registers.HL.hi);
            break;
        case 0x45: // LD B, L
            timer += instSet->loadReg8(&registers.BC.hi, &registers.HL.lo);
            break;
        case 0x46: // LD B, (HL)
            timer += instSet->loadReg8HL(&registers.BC.hi); 
            break;
        case 0x47: // LD B, A
            timer += instSet->loadReg8(&registers.BC.hi, &registers.AF.hi);
            break;
        case 0x48: // LD C, B
            timer += instSet->loadReg8(&registers.BC.lo, &registers.BC.hi);
            break;
        case 0x49: // LD C, C
            timer += instSet->loadReg8(&registers.BC.lo, &registers.BC.lo);
            break;
        case 0x4A: // LD C, D
            timer += instSet->loadReg8(&registers.BC.lo, &registers.DE.hi);
            break;
        case 0x4B:// LD C, E
            timer += instSet->loadReg8(&registers.BC.lo, &registers.DE.lo);
            break;
        case 0x4C: // LD C, H
            timer += instSet->loadReg8(&registers.BC.lo, &registers.HL.hi);
            break;
        case 0x4D: // LD C, L
            timer += instSet->loadReg8(&registers.BC.lo, &registers.HL.lo);
            break;
        case 0x4E: // LD C, (HL)
            timer += instSet->loadReg8HL(&registers.BC.lo); 
            break;
        case 0x4F: // LD C, A
            timer += instSet->loadReg8(&registers.BC.lo, &registers.AF.hi);
            break;
        case 0x50: // LD D, B
            timer += instSet->loadReg8(&registers.DE.hi, &registers.BC.hi);
            break;
        case 0x51: // LD D, C
            timer += instSet->loadReg8(&registers.DE.hi, &registers.BC.lo);
            break;
        case 0x52: // LD D, D
            timer += instSet->loadReg8(&registers.DE.hi, &registers.DE.hi);
            break;
        case 0x53: // LD D, E
            timer += instSet->loadReg8(&registers.DE.hi, &registers.DE.lo);
            break;
        case 0x54: // LD D, H
            timer += instSet->loadReg8(&registers.DE.hi, &registers.HL.hi);
            break;
        case 0x55: // LD D, L
            timer += instSet->loadReg8(&registers.DE.hi, &registers.HL.lo);
            break;
        case 0x56: // LD D, (HL)
            timer += instSet->loadReg8HL(&registers.DE.hi); 
            break;
        case 0x57: // LD D, A
            timer += instSet->loadReg8(&registers.DE.hi, &registers.AF.hi);
            break;
        case 0x58: // LD E, B
            timer += instSet->loadReg8(&registers.DE.lo, &registers.BC.hi);
            break;
        case 0x59: // LD E, C
            timer += instSet->loadReg8(&registers.DE.lo, &registers.BC.lo);
            break;
        case 0x5A: // LD E, D
            timer += instSet->loadReg8(&registers.DE.lo, &registers.DE.hi);
            break;
        case 0x5B: // LD E, E
            timer += instSet->loadReg8(&registers.DE.lo, &registers.DE.lo);
            break;
        case 0x5C: // LD E, H
            timer += instSet->loadReg8(&registers.DE.lo, &registers.HL.hi);
            break;
        case 0x5D: // LD E, L
            timer += instSet->loadReg8(&registers.DE.lo, &registers.HL.lo);
            break;
        case 0x5E: // LD E, (HL)
            timer += instSet->loadReg8HL(&registers.DE.lo); 
            break;
        case 0x5F: // LD E, A
            timer += instSet->loadReg8(&registers.DE.lo, &registers.BC.hi);
            break;
        case 0x60: // LD H, B
            timer += instSet->loadReg8(&registers.HL.hi, &registers.BC.hi);
            break;
        case 0x61: // LD H, C
            timer += instSet->loadReg8(&registers.HL.hi, &registers.BC.lo);
            break;
        case 0x62: // LD H, D
            timer += instSet->loadReg8(&registers.HL.hi, &registers.DE.hi);
            break;
        case 0x63: // LD H, E
            timer += instSet->loadReg8(&registers.HL.hi, &registers.DE.lo);
            break;
        case 0x64: // LD H, H
            timer += instSet->loadReg8(&registers.HL.hi, &registers.HL.hi);
            break;
        case 0x65: // LD H, L
            timer += instSet->loadReg8(&registers.HL.hi, &registers.HL.lo);
            break;
        case 0x66: // LD H, (HL)
            timer += instSet->loadReg8HL(&registers.HL.hi); 
            break;
        case 0x67: // LD H, A
            timer += instSet->loadReg8(&registers.HL.hi, &registers.AF.hi);
            break;
        case 0x68: // LD L, B
            timer += instSet->loadReg8(&registers.HL.lo, &registers.BC.hi);
            break;
        case 0x69: // LD L, C
            timer += instSet->loadReg8(&registers.HL.lo, &registers.BC.lo);
            break;
        case 0x6A: // LD L, D
            timer += instSet->loadReg8(&registers.HL.lo, &registers.DE.hi);
            break;
        case 0x6B: // LD L, E
            timer += instSet->loadReg8(&registers.HL.lo, &registers.DE.lo);
            break;
        case 0x6C: // LD L, H
            timer += instSet->loadReg8(&registers.HL.lo, &registers.HL.hi);
            break;
        case 0x6D: // LD L, L
            timer += instSet->loadReg8(&registers.HL.lo, &registers.HL.lo);
            break;
        case 0x6E: // LD L, (HL)
            timer += instSet->loadReg8HL(&registers.HL.lo); 
            break;
        case 0x6F: // LD L, A
            timer += instSet->loadReg8(&registers.HL.lo, &registers.AF.hi);
            break;
        case 0x70: // LD (HL), B
            timer += instSet->storeReg8HL(registers.BC.hi); 
            break;
        case 0x71: // LD (HL), C
            timer += instSet->storeReg8HL(registers.BC.lo);
            break;
        case 0x72: // LD (HL), D
            timer += instSet->storeReg8HL(registers.DE.hi);
            break;
        case 0x73: // LD (HL), E
            timer += instSet->storeReg8HL(registers.DE.lo);
            break;
        case 0x74: // LD (HL), H
            timer += instSet->storeReg8HL(registers.HL.hi);
            break;
        case 0x75: // LD (HL), L
            timer += instSet->storeReg8HL(registers.HL.lo);
            break;
        case 0x76: // HALT
            timer += instSet->halt(); 
            break;
        case 0x77: // LD (HL), A
            timer += instSet->storeReg8HL(registers.AF.hi);
            break;
        case 0x78: // LD A, B
            timer += instSet->loadReg8(&registers.AF.hi, &registers.BC.hi);
            break;
        case 0x79: // LD A, C
            timer += instSet->loadReg8(&registers.AF.hi, &registers.BC.lo);
            break;
        case 0x7A: // LD A, D
            timer += instSet->loadReg8(&registers.AF.hi, &registers.DE.hi);
            break;
        case 0x7B: // LD A, E
            timer += instSet->loadReg8(&registers.AF.hi, &registers.DE.lo);
            break;
        case 0x7C: // LD A, H
            timer += instSet->loadReg8(&registers.AF.hi, &registers.HL.hi);
            break;
        case 0x7D: // LD A, L
            timer += instSet->loadReg8(&registers.AF.hi, &registers.HL.lo);
            break;
        case 0x7E: // LD A, (HL)
            timer += instSet->loadReg8HL(&registers.AF.hi); 
            break;
        case 0x7F: // LD A, A
            timer += instSet->loadReg8(&registers.AF.hi, &registers.AF.hi);
                break;
        case 0x80: // ADD A, B
            timer += instSet->addReg(registers.BC.hi); 
            break;
        case 0x81: // ADD A, C
            timer += instSet->addReg(registers.BC.lo);
            break;
        case 0x82: // ADD A, D
            timer += instSet->addReg(registers.DE.hi);
            break;
        case 0x83: // ADD A, E
            timer += instSet->addReg(registers.DE.lo);
            break;
        case 0x84: // ADD A, H
            timer += instSet->addReg(registers.HL.hi);
            break;
        case 0x85: // ADD A, L
            timer += instSet->addReg(registers.HL.lo);
            break;
        case 0x86: // ADD A, (HL)
            timer += instSet->addHLInd(); 
            break;
        case 0x87: // ADD A, A
            timer += instSet->addReg(registers.AF.hi);
            break;
        case 0x88: // ADC A, B
            timer += instSet->adcReg(registers.BC.hi); 
            break;
        case 0x89: // ADC A, C
            timer += instSet->adcReg(registers.BC.lo);
            break;
        case 0x8A: // ADC A, D
            timer += instSet->adcReg(registers.DE.hi);
            break;
        case 0x8B: // ADC A, E
            timer += instSet->adcReg(registers.DE.lo);
            break;
        case 0x8C: // ADC A, H
            timer += instSet->adcReg(registers.HL.hi);
            break;
        case 0x8D: // ADC A, L
            timer += instSet->adcReg(registers.HL.lo);
            break;
        case 0x8E: // ADC A, (HL)
            timer += instSet->adcHLInd(); 
            break;
        case 0x8F: // ADC A, A
            timer += instSet->adcReg(registers.AF.hi);
            break;
        case 0x90: // SUB B
            timer += instSet->subReg(registers.BC.hi);
            break;
        case 0x91: // SUB C
            timer += instSet->subReg(registers.BC.lo);
            break;
        case 0x92: // SUB D
            timer += instSet->subReg(registers.DE.hi);
            break;
        case 0x93: // SUB E
            timer += instSet->subReg(registers.DE.lo);
            break;
        case 0x94: // SUB H
            timer += instSet->subReg(registers.HL.hi);
            break;
        case 0x95: // SUB L
            timer += instSet->subReg(registers.HL.lo);
            break;
        case 0x96: // SUB (HL)
            timer += instSet->subHLInd(); 
            break;
        case 0x97: // SUB A
            timer += instSet->subReg(registers.AF.hi);
            break;
        case 0x98: // SBC A, B
            timer += instSet->sbcReg(registers.BC.hi);
            break;
        case 0x99: // SBC A, C
            timer += instSet->sbcReg(registers.BC.lo);
            break;
        case 0x9A: // SBC A, D
            timer += instSet->sbcReg(registers.DE.hi);
            break;
        case 0x9B: // SBC A, E
            timer += instSet->sbcReg(registers.DE.lo);
            break;
        case 0x9C: // SBC A, H
            timer += instSet->sbcReg(registers.HL.hi);
            break;
        case 0x9D: // SBC A, L
            timer += instSet->sbcReg(registers.HL.lo);
            break;
        case 0x9E: // SBC A, (HL)
            timer += instSet->subHLInd(); 
            break;
        case 0x9F: // SBC A, A
            timer += instSet->sbcReg(registers.AF.hi);
            break;
        case 0xA0: // AND B
            timer += instSet->andReg(registers.BC.hi); 
            break;
        case 0xA1: // AND C
            timer += instSet->andReg(registers.BC.lo);
            break;
        case 0xA2: // AND D
            timer += instSet->andReg(registers.DE.hi);
            break;
        case 0xA3: // AND E
            timer += instSet->andReg(registers.DE.lo);
            break;
        case 0xA4: // AND H
            timer += instSet->andReg(registers.HL.hi);
            break;
        case 0xA5: // AND L
            timer += instSet->andReg(registers.HL.lo);
            break;
        case 0xA6: // AND (HL)
            timer += instSet->andHLInd(); 
            break;
        case 0xA7: // AND A
            timer += instSet->andReg(registers.AF.hi);
            break;
        case 0xA8: // XOR B 
            timer += instSet->xorReg(registers.BC.hi);
            break;
        case 0xA9: // XOR C
            timer += instSet->xorReg(registers.BC.lo);
            break;
        case 0xAA: // XOR D
            timer += instSet->xorReg(registers.DE.hi);
            break;
        case 0xAB: // XOR E
            timer += instSet->xorReg(registers.DE.lo);
            break;
        case 0xAC: // XOR H
            timer += instSet->xorReg(registers.HL.hi);
            break;
        case 0xAD: // XOR L
            timer += instSet->xorReg(registers.HL.lo);
            break;
        case 0xAE: // XOR (HL)
            timer += instSet->xorHLInd(); 
            break;
        case 0xAF: // XOR A
            timer += instSet->xorReg(registers.AF.hi);
            break;
        case 0xB0: // OR B
            timer += instSet->orReg(registers.BC.hi);
            break;
        case 0xB1: // OR C
            timer += instSet->orReg(registers.BC.lo);
            break;
        case 0xB2: // OR D
            timer += instSet->orReg(registers.DE.hi);
            break;
        case 0xB3: // OR E
            timer += instSet->orReg(registers.DE.lo);
            break;
        case 0xB4: // OR H
            timer += instSet->orReg(registers.HL.hi);
            break;
        case 0xB5: // OR L
            timer += instSet->orReg(registers.HL.lo);
            break;
        case 0xB6: // OR (HL)
            timer += instSet->orHLInd(); 
            break;
        case 0xB7: // OR A
            timer += instSet->orReg(registers.AF.hi);
            break;
        case 0xB8: // CP B
            timer += instSet->compareReg(registers.BC.hi);
            break;
        case 0xB9: // CP C
            timer += instSet->compareReg(registers.BC.lo);
            break;
        case 0xBA: // CP D
            timer += instSet->compareReg(registers.DE.hi);
            break;
        case 0xBB: // CP E
            timer += instSet->compareReg(registers.DE.lo);
            break;
        case 0xBC: // CP H
            timer += instSet->compareReg(registers.HL.hi);
            break;
        case 0xBD: // CP L
            timer += instSet->compareReg(registers.HL.lo);
            break;
        case 0xBE: // CP (HL)
            timer += instSet->compareHLInd(); 
            break;
        case 0xBF: // CP A
            timer += instSet->compareReg(registers.AF.hi);
            break;
        case 0xC0: // RET NZ
            timer += instSet->conditionalReturnPC(flags->Z == 0); 
            break;
        case 0xC1: // POP BC
            timer += instSet->popFromStack(&registers.BC);
            break;
        case 0xC2: // JP NZ, nn
            timer += instSet->conditionalJump(flags->Z == 0); 
            break;
        case 0xC3: // JP nn
            timer += instSet->jump();
            break;
        case 0xC4: // CALL NZ, nn
            timer += instSet->conditionalCall(flags->Z == 0); 
            break;
        case 0xC5: // PUSH BC
            timer += instSet->pushToStack(registers.BC);
            break;
        case 0xC6: // ADD A, n
            timer += instSet->addImm(); 
            break;
        case 0xC7: // RST 0
            timer += instSet->sysCall(0x00); 
            break;
        case 0xC8: // RET Z
            timer += instSet->conditionalReturnPC(flags->Z == 1);
            break;
        case 0xC9: // RET
            timer += instSet->returnPC();
            break;
        case 0xCA: // JP Z, nn
            timer += instSet->conditionalJump(flags->Z == 1);
            break;
        case 0xCB: // Prefix
            executeCBInstruction();
            break;
        case 0xCC: // CALL Z, nn
            timer += instSet->conditionalCall(flags->Z == 1);
            break;
        case 0xCD: // CALL nn
            timer += instSet->call(); 
            break;
        case 0xCE: // ADC A, n
            timer += instSet->adcImm();
            break;
        case 0xCF: // RST 8
            timer += instSet->sysCall(0x08);
            break;
        case 0xD0: // RET NC
            timer += instSet->conditionalReturnPC(flags->C == 0);
            break;
        case 0xD1: // POP DE
            timer += instSet->popFromStack(&registers.DE);
            break;
        case 0xD2: // JP NC, nn
            timer += instSet->conditionalJump(flags->C == 0);
            break;
        case 0xD4: // CALL NC, nn
            timer += instSet->conditionalCall(flags->C == 0); 
            break;
        case 0xD5: // PUSH DE
            timer += instSet->pushToStack(registers.DE);
            break;
        case 0xD6: // SUB n
            timer += instSet->subImm();
            break;
        case 0xD7: // RST 10H
            timer += instSet->sysCall(0x10);
            break;
        case 0xD8: // RET C
            timer += instSet->conditionalReturnPC(flags->C == 0); 
            break;
        case 0xD9: // RETI
            timer += instSet->returnPCI(&intMasterEnable);
            break;
        case 0xDA: // JP C, nn
            timer += instSet->conditionalJump(flags->C == 0); 
            break;
        case 0xDC: // CALL C, nn
            timer += instSet->conditionalCall(flags->C == 0);
            break;
        case 0xDE: // SBC A, n
            timer += instSet->sbcImm(); 
            break;
        case 0xDF: // RST 18H
            timer += instSet->sysCall(0x18); 
            break;
        case 0xE0: // LD (FF00+n), A
            timer += instSet->writeIOPortN();
            break;
        case 0xE1: // POP HL
            timer += instSet->popFromStack(&registers.HL);
            break;
        case 0xE2: // LD (FF00+C), A
            timer += instSet->writeIOPortC();
            break;
        case 0xE5: // PUSH HL
            timer += instSet->pushToStack(registers.HL); 
            break;
        case 0xE6: // AND n
            timer += instSet->andImm();
            break;
        case 0xE7: // RST 20H
            timer += instSet->sysCall(0x20);
            break;
        case 0xE8: // ADD SP, dd
            timer += instSet->addImmToSP();
            break;
        case 0xE9: // JP (HL)
            timer += instSet->jumpHL(); 
            break;
        case 0xEA: // LD (nn), A
            timer += instSet->storeAInd(); 
            break;
        case 0xEE: // XOR n
            timer += instSet->xorImm(); 
            break;
        case 0xEF: // RST 28H
            timer += instSet->sysCall(0x28);
            break;
        case 0xF0: // LD A, (FF00+n)
            timer += instSet->readIOPortN();
            break;
        case 0xF1: // POP AF
            timer += instSet->popFromStack(&registers.AF);
            break;
        case 0xF2: // LD A, (FF00+C)
            timer += instSet->readIOPortC(); 
            break;
        case 0xF3: // DI
            timer += instSet->disableInterrupts(&intMasterEnable); 
            break;
        case 0xF5: // PUSH AF
            timer += instSet->pushToStack(registers.AF.hi);
            break;
        case 0xF6: // OR n
            timer += instSet->orImm(); 
            break;
        case 0xF7: // RST 30H
            timer += instSet->sysCall(0x30);
            break;
        case 0xF8: // LD HL, SP+dd
            timer += instSet->addSPToHL();
            break;
        case 0xF9: // LD SP, HL
            timer += instSet->loadHLToSP();
            break;
        case 0xFA: // LD A, (nn)
            timer += instSet->loadAInd(); 
            break;
        case 0xFB: // EI
            timer += instSet->enableInterrupts(&intMasterEnable);
            break;
        case 0xFE: // CP n
            timer += instSet->compareImm();
            break;
        case 0xFF: // RST 38H
            timer += instSet->sysCall(0x38);
            break;
    }
}

void Z80Cpu::executeCBInstruction()
{
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

    // find out which value is going to be used
    switch(lowInst)
    {
        case 0x0:
            reg = &registers.BC.hi;
            break;
        case 0x1:
            reg = &registers.BC.lo;
            break;
        case 0x2:
            reg = &registers.DE.hi;
            break;
        case 0x3:
            reg = &registers.DE.lo;
            break;
        case 0x4:
            reg = &registers.HL.hi;
            break;
        case 0x5:
            reg = &registers.HL.lo;
            break;
        case 0x6:
            // (HL) is being used, grab it from memory and set the flag
            isInd = 1;
            hlMem = memory->read(registers.HL.val);
            reg = &hlMem;
            break;
        case 0x7:
            reg = &registers.AF.hi;
            break;
        case 0x8:
            reg = &registers.BC.hi;
            break;
        case 0x9:
            reg = &registers.BC.lo;
            break;
        case 0xA:
            reg = &registers.DE.hi;
            break;
        case 0xB:
            reg = &registers.DE.lo;
            break;
        case 0xC:
            reg = &registers.HL.hi;
            break;
        case 0xD:
            reg = &registers.HL.lo;
            break;
        case 0xE:
            // (HL) is being used, grab it from memory and set the flag
            isInd = 1;
            hlMem = memory->read(registers.HL.val);
            reg = &hlMem;
            break;
        case 0xF:
            reg = &registers.AF.hi;
            break;
    }
    
    int timer = 0;
    switch(highInst)
    {
        case 0x0:
            if(lowInst < 8) // RLC
                timer += instSet->rotateRegLeftC(reg);
            else            // RRC
                timer += instSet->rotateRegRightC(reg);
            break;
        case 0x1:
            if(lowInst < 8) // RL
                timer += instSet->rotateRegLeft(reg);
            else            // RR
                 timer += instSet->rotateRegRight(reg);
            break;
        case 0x2:
            if(lowInst < 8) // SLA
                timer += instSet->shiftRegLeftA(reg);
            else            // SRA
                timer += instSet->shiftRegRightA(reg);
            break;            
        case 0x3:
            if(lowInst < 8) // SWAP
                timer += instSet->swapReg(reg);
            else            // SRL
                timer += instSet->shiftRegRightL(reg);
            break;
        case 0x4:           // BIT
        case 0x5:
        case 0x6:
        case 0x7:
            if (lowInst < 8)
                timer += instSet->testRegBit(reg, (highInst - 4) * 2);
            else
                timer += instSet->testRegBit(reg, ((highInst - 4) * 2) + 1);
            // test bit does not need to write back to memory.
            memWrite = 0;
            break;
        case 0x8:            // RES
        case 0x9:
        case 0xA:
        case 0xB:
            if (lowInst < 8)
                timer += instSet->resetRegBit(reg, (highInst - 8) * 2);
            else
                timer += instSet->resetRegBit(reg, ((highInst - 8) * 2) + 1);
            break;
        case 0xC:             // SET
        case 0xD:
        case 0xE:
        case 0xF:
            if (lowInst < 8)
                timer += instSet->resetRegBit(reg, (highInst - 0xC) * 2);
            else
                timer += instSet->resetRegBit(reg, ((highInst - 0xC) * 2) + 1);
            break;
    }
    
    // (HL) was used
    if(isInd)
    {
        // add time needed to read from memory
        timer += 4;
        // check if we need to write back to memory.
        if(memWrite)
        {
            memory->write(registers.HL.val, hlMem);
            // add time to write to memory 
            timer += 4;
        }
    } 
}
