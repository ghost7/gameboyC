#include <stdint.h>
#include <stdio.h>
#include "cpu_z80.h"
#include "cpu_z80_inst.h"
#include "memory.h"
#include "registers.h"
#ifdef DEBUG
	#include "debug.h"
#endif

static int cbPrefix();
static int getFRegister();
static void setFRegister(int8_t value);
static int8_t registerF;
static int ticks = 0;

/**
 * Initialize registers to their default values.
 */
void initCpu()
{
	registers.A = 0x01;
	registers.B = 0x00;
	registers.C = 0x13;
	registers.D = 0x00;
	registers.E = 0xD8;
	setFRegister(0xB0);
	registers.H = 0x01;
	registers.L = 0x4D;
	registers.PC = 0x00;
	registers.SP = 0xFFFE;
}


int checkInterrupts()
{
	// Mask off interrupts that are not enabled.
	uint8_t interruptState = register_IE & register_IF;     // IE & IF
	int ticks = 0;

	if(interruptState && intMasEnable)
	{
		switch(interruptState)
		{
			case 0x01:                 // Vertical Blank
				ticks += sysCall(0x40);
				ioRegisters[0x0F] = 0;
				break;
			case 0x02:                 // LCD status triggers
				ticks += sysCall(0x48);
				ioRegisters[0x0F] = 0;
				break;
			case 0x04:                 // Timer overflow
				ticks += sysCall(0x50);
				ioRegisters[0x0F] = 0;
				break;
			case 0x08:                 // Serial link
				ticks += sysCall(0x58);
				ioRegisters[0x0F] = 0;
				break;
			case 0x10:                 // Joypad State
				ticks += sysCall(0x60);
				ioRegisters[0x0F] = 0;
				break;
		}
	}
	return ticks;
}

/**
 * Execute the next instruction.
 */
int cpuStep()
{
	uint8_t cpuInst = memoryRead(registers.PC++);

#ifdef DEBUG
	printInst(registers.PC - 1, cpuInst);
#endif

	int ticks = 0;	
	switch(cpuInst)
	{
		case 0x00: // NOP
			ticks += 4;
			break;
		case 0x01: // LD BC, NN
			ticks += loadReg16(&registers.B, &registers.C);	
			break;
		case 0x02: // LD (BC), A
			ticks += storeA(registers.B, registers.C);	
			break;
		case 0x03: // INC BC
			ticks += incReg16(&registers.B, &registers.C);
			break;
		case 0x04: // INC B
			ticks += incReg8(&registers.B);
			break;
		case 0x05: // DEC B
			ticks += decReg8(&registers.B);
			break;
		case 0x06: // LD B, n
			ticks += loadImmReg8(&registers.B);
			break;
		case 0x07: // RLCA
			ticks += rotateALeftC(); 
			break;
		case 0x08: // LD (nn), SP
			ticks +=  loadSPImm();
			break;
		case 0x09: // ADD HL, BC
			ticks += addReg16(registers.B, registers.C);
			break;
		case 0x0A: // LD A, (BC)
			ticks += loadA(registers.B, registers.C);
			break;
		case 0x0B: // DEC BC
			ticks += decReg16(&registers.B, &registers.C);
			break;
		case 0x0C: // INC C
			ticks += incReg8(&registers.C);
			break;
		case 0x0D: // DEC C
			ticks += decReg8(&registers.C);
			break;
		case 0x0E: // LD C, n
			ticks += loadImmReg8(&registers.C); 
			break;
		case 0x0F: // RRCA
			ticks += rotateARightC();
			break;
		case 0x10: // STOP
			ticks += stop(); 
			break;
		case 0x11: // LD DE, nn
			ticks += loadReg16(&registers.D, &registers.E); 
			break;
		case 0x12: // LD (DE), A
			ticks += storeA(registers.D, registers.E); 
			break;
		case 0x13: // INC DE
			ticks += incReg16(&registers.D, &registers.E);
			break;
		case 0x14: // INC D
			ticks += incReg8(&registers.D);
			break;
		case 0x15: // DEC D
			ticks += decReg8(&registers.D);
			break;
		case 0x16: // LD D, n
			ticks += loadImmReg8(&registers.D);
			break;
		case 0x17: // RLA
			ticks += rotateALeft(); 
			break;
		case 0x18: // JR d
			ticks += relativeJump();
			break;
		case 0x19: // ADD HL, DE
			ticks += addReg16(registers.D, registers.E);
			break;
		case 0x1A: // LD A, (DE)
			ticks += loadA(registers.D, registers.E);
			break;
		case 0x1B: // DEC DE
			ticks += decReg16(&registers.D, &registers.E);
			break;
		case 0x1C: // INC E
			ticks += incReg8(&registers.E);
			break;
		case 0x1D: // DEC E 
			ticks += decReg8(&registers.E);
			break;
		case 0x1E: // LD E, n
			ticks += loadImmReg8(&registers.E);
			break;
		case 0x1F: // RRA
			ticks += rotateARight();
			break;
		case 0x20: // JR NZ, d 
			ticks += conditionalRelativeJump(flags.Z == 0);  
			break;
		case 0x21: // LD HL, nn
			ticks += loadReg16(&registers.H, &registers.L);
			break;
		case 0x22: // LDI (HL), A
			ticks += storeIncrement();  
			break;
		case 0x23: // INC HL
			ticks += incReg16(&registers.H, &registers.L);
			break;
		case 0x24: // INC H
			ticks += incReg8(&registers.H);
			break;
		case 0x25: // DEC H
			ticks += decReg8(&registers.H);
			break;
		case 0x26: // LD H, n
			ticks += loadImmReg8(&registers.H);
			break;
		case 0x27: // DAA
			ticks += decimalyAdjustA();
			break;
		case 0x28: // JR Z, d
			ticks += conditionalRelativeJump(flags.Z == 1); 
			break;
		case 0x29: // ADD HL, HL
			ticks += addReg16(registers.H, registers.L);
			break;
		case 0x2A: // LDI A, (HL)
			ticks += loadIncrement();
			break;
		case 0x2B: // DEC HL
			ticks += decReg16(&registers.H, &registers.L); 
			break;
		case 0x2C: // INC L
			ticks += incReg8(&registers.L); 
			break;
		case 0x2D: // DEC L
			ticks += decReg8(&registers.L);
			break;
		case 0x2E: // LD L, n
			ticks += loadImmReg8(&registers.L);
			break;
		case 0x2F: // CPL
			ticks += complementA();
			break;
		case 0x30: // JR NC, d
			ticks += conditionalRelativeJump(flags.C == 0);
			break;
		case 0x31: // LD SP, nn
			ticks += loadSPImm();
			break;
		case 0x32: // LD (nn), A
			ticks += storeDecrement();
			break;
		case 0x33: // INC SP
			registers.SP++;
			ticks += 8;
			break;
		case 0x34: // INC (HL)
			ticks += incHLInd();
			break;
		case 0x35: // DEC (HL)
			ticks += decHLInd(); 
			break;
		case 0x36: // LD (HL), n
			ticks += storeHLImm();
			break;
		case 0x37: // SCF
			ticks += setCarry(); 
			break;
		case 0x38: // JR C, d
			ticks += conditionalRelativeJump(flags.C == 0);
			break;
		case 0x39: // ADD HL, SP
			ticks += addReg16((registers.SP >> 8) & 0xFF, registers.SP & 0xFF); 
			break;
		case 0x3A: // LDD A, (HL)
			ticks += loadDecrement(); 
			break;
		case 0x3B: // DEC SP
			registers.SP--;
			ticks += 8;
			break; 
		case 0x3C: // INC A
			ticks += incReg8(&registers.A); 
			break;
		case 0x3D: // DEC A
			ticks += decReg8(&registers.A);
			break;
		case 0x3E: // LD A, n
			ticks += loadImmReg8(&registers.A);
			break;
		case 0x3F: // CCF
			ticks += complementCarry();
			break;
		case 0x40: // LD B, B
			ticks += loadReg8(&registers.B, &registers.B);
			break;
		case 0x41: // LD B, C
			ticks += loadReg8(&registers.B, &registers.C);
			break;
		case 0x42: // LD B, D
			ticks += loadReg8(&registers.B, &registers.D);
			break;
		case 0x43: // LD B, E
			ticks += loadReg8(&registers.B, &registers.E);
			break;
		case 0x44: // LD B, H
			ticks += loadReg8(&registers.B, &registers.H);
			break;
		case 0x45: // LD B, L
			ticks += loadReg8(&registers.B, &registers.L);
			break;
		case 0x46: // LD B, (HL)
			ticks += loadReg8HL(&registers.B); 
			break;
		case 0x47: // LD B, A
			ticks += loadReg8(&registers.B, &registers.A);
			break;
		case 0x48: // LD C, B
			ticks += loadReg8(&registers.C, &registers.B);
			break;
		case 0x49: // LD C, C
			ticks += loadReg8(&registers.C, &registers.C);
			break;
		case 0x4A: // LD C, D
			ticks += loadReg8(&registers.C, &registers.D);
			break;
		case 0x4B:// LD C, E
			ticks += loadReg8(&registers.C, &registers.E);
			break;
		case 0x4C: // LD C, H
			ticks += loadReg8(&registers.C, &registers.H);
			break;
		case 0x4D: // LD C, L
			ticks += loadReg8(&registers.C, &registers.L);
			break;
		case 0x4E: // LD C, (HL)
			ticks += loadReg8HL(&registers.C); 
			break;
		case 0x4F: // LD C, A
			ticks += loadReg8(&registers.C, &registers.A);
			break;
		case 0x50: // LD D, B
			ticks += loadReg8(&registers.D, &registers.B);
			break;
		case 0x51: // LD D, C
			ticks += loadReg8(&registers.D, &registers.C);
			break;
		case 0x52: // LD D, D
			ticks += loadReg8(&registers.D, &registers.D);
			break;
		case 0x53: // LD D, E
			ticks += loadReg8(&registers.D, &registers.E);
			break;
		case 0x54: // LD D, H
			ticks += loadReg8(&registers.D, &registers.H);
			break;
		case 0x55: // LD D, L
			ticks += loadReg8(&registers.D, &registers.L);
			break;
		case 0x56: // LD D, (HL)
			ticks += loadReg8HL(&registers.D); 
			break;
		case 0x57: // LD D, A
			ticks += loadReg8(&registers.D, &registers.A);
			break;
		case 0x58: // LD E, B
			ticks += loadReg8(&registers.E, &registers.B);
			break;
		case 0x59: // LD E, C
			ticks += loadReg8(&registers.E, &registers.C);
			break;
		case 0x5A: // LD E, D
			ticks += loadReg8(&registers.E, &registers.D);
			break;
		case 0x5B: // LD E, E
			ticks += loadReg8(&registers.E, &registers.E);
			break;
		case 0x5C: // LD E, H
			ticks += loadReg8(&registers.E, &registers.H);
			break;
		case 0x5D: // LD E, L
			ticks += loadReg8(&registers.E, &registers.L);
			break;
		case 0x5E: // LD E, (HL)
			ticks += loadReg8HL(&registers.E); 
			break;
		case 0x5F: // LD E, A
			ticks += loadReg8(&registers.E, &registers.B);
			break;
		case 0x60: // LD H, B
			ticks += loadReg8(&registers.H, &registers.B);
			break;
		case 0x61: // LD H, C
			ticks += loadReg8(&registers.H, &registers.C);
			break;
		case 0x62: // LD H, D
			ticks += loadReg8(&registers.H, &registers.D);
			break;
		case 0x63: // LD H, E
			ticks += loadReg8(&registers.H, &registers.E);
			break;
		case 0x64: // LD H, H
			ticks += loadReg8(&registers.H, &registers.H);
			break;
		case 0x65: // LD H, L
			ticks += loadReg8(&registers.H, &registers.L);
			break;
		case 0x66: // LD H, (HL)
			ticks += loadReg8HL(&registers.H); 
			break;
		case 0x67: // LD H, A
			ticks += loadReg8(&registers.H, &registers.A);
			break;
		case 0x68: // LD L, B
			ticks += loadReg8(&registers.L, &registers.B);
			break;
		case 0x69: // LD L, C
			ticks += loadReg8(&registers.L, &registers.C);
			break;
		case 0x6A: // LD L, D
			ticks += loadReg8(&registers.L, &registers.D);
			break;
		case 0x6B: // LD L, E
			ticks += loadReg8(&registers.L, &registers.E);
			break;
		case 0x6C: // LD L, H
			ticks += loadReg8(&registers.L, &registers.H);
			break;
		case 0x6D: // LD L, L
			ticks += loadReg8(&registers.L, &registers.L);
			break;
		case 0x6E: // LD L, (HL)
			ticks += loadReg8HL(&registers.L); 
			break;
		case 0x6F: // LD L, A
			ticks += loadReg8(&registers.L, &registers.A);
			break;
		case 0x70: // LD (HL), B
			ticks += storeReg8HL(registers.B); 
			break;
		case 0x71: // LD (HL), C
			ticks += storeReg8HL(registers.C);
			break;
		case 0x72: // LD (HL), D
			ticks += storeReg8HL(registers.D);
			break;
		case 0x73: // LD (HL), E
			ticks += storeReg8HL(registers.E);
			break;
		case 0x74: // LD (HL), H
			ticks += storeReg8HL(registers.H);
			break;
		case 0x75: // LD (HL), L
			ticks += storeReg8HL(registers.L);
			break;
		case 0x76: // HALT
			ticks += halt(); 
			break;
		case 0x77: // LD (HL), A
			ticks += storeReg8HL(registers.A);
			break;
		case 0x78: // LD A, B
			ticks += loadReg8(&registers.A, &registers.B);
			break;
		case 0x79: // LD A, C
			ticks += loadReg8(&registers.A, &registers.C);
			break;
		case 0x7A: // LD A, D
			ticks += loadReg8(&registers.A, &registers.D);
			break;
		case 0x7B: // LD A, E
			ticks += loadReg8(&registers.A, &registers.E);
			break;
		case 0x7C: // LD A, H
			ticks += loadReg8(&registers.A, &registers.H);
			break;
		case 0x7D: // LD A, L
			ticks += loadReg8(&registers.A, &registers.L);
			break;
		case 0x7E: // LD A, (HL)
			ticks += loadReg8HL(&registers.A); 
			break;
		case 0x7F: // LD A, A
			ticks += loadReg8(&registers.A, &registers.A);
				break;
		case 0x80: // ADD A, B
			ticks += addReg(registers.B); 
			break;
		case 0x81: // ADD A, C
			ticks += addReg(registers.C);
			break;
		case 0x82: // ADD A, D
			ticks += addReg(registers.D);
			break;
		case 0x83: // ADD A, E
			ticks += addReg(registers.E);
			break;
		case 0x84: // ADD A, H
			ticks += addReg(registers.H);
			break;
		case 0x85: // ADD A, L
			ticks += addReg(registers.L);
			break;
		case 0x86: // ADD A, (HL)
			ticks += addHLInd(); 
			break;
		case 0x87: // ADD A, A
			ticks += addReg(registers.A);
			break;
		case 0x88: // ADC A, B
			ticks += adcReg(registers.B); 
			break;
		case 0x89: // ADC A, C
			ticks += adcReg(registers.C);
			break;
		case 0x8A: // ADC A, D
			ticks += adcReg(registers.D);
			break;
		case 0x8B: // ADC A, E
			ticks += adcReg(registers.E);
			break;
		case 0x8C: // ADC A, H
			ticks += adcReg(registers.H);
			break;
		case 0x8D: // ADC A, L
			ticks += adcReg(registers.L);
			break;
		case 0x8E: // ADC A, (HL)
			ticks += adcHLInd(); 
			break;
		case 0x8F: // ADC A, A
			ticks += adcReg(registers.A);
			break;
		case 0x90: // SUB B
			ticks += subReg(registers.B);
			break;
		case 0x91: // SUB C
			ticks += subReg(registers.C);
			break;
		case 0x92: // SUB D
			ticks += subReg(registers.D);
			break;
		case 0x93: // SUB E
			ticks += subReg(registers.E);
			break;
		case 0x94: // SUB H
			ticks += subReg(registers.H);
			break;
		case 0x95: // SUB L
			ticks += subReg(registers.L);
			break;
		case 0x96: // SUB (HL)
			ticks += subHLInd(); 
			break;
		case 0x97: // SUB A
			ticks += subReg(registers.B);
			break;
		case 0x98: // SBC A, B
			ticks += sbcReg(registers.B);
			break;
		case 0x99: // SBC A, C
			ticks += sbcReg(registers.C);
			break;
		case 0x9A: // SBC A, D
			ticks += sbcReg(registers.D);
			break;
		case 0x9B: // SBC A, E
			ticks += sbcReg(registers.E);
			break;
		case 0x9C: // SBC A, H
			ticks += sbcReg(registers.H);
			break;
		case 0x9D: // SBC A, L
			ticks += sbcReg(registers.L);
			break;
		case 0x9E: // SBC A, (HL)
			ticks += subHLInd(); 
			break;
		case 0x9F: // SBC A, A
			ticks += sbcReg(registers.A);
			break;
		case 0xA0: // AND B
			ticks += andReg(registers.B); 
			break;
		case 0xA1: // AND C
			ticks += andReg(registers.C);
			break;
		case 0xA2: // AND D
			ticks += andReg(registers.D);
			break;
		case 0xA3: // AND E
			ticks += andReg(registers.E);
			break;
		case 0xA4: // AND H
			ticks += andReg(registers.H);
			break;
		case 0xA5: // AND L
			ticks += andReg(registers.L);
			break;
		case 0xA6: // AND (HL)
			ticks += andHLInd(); 
			break;
		case 0xA7: // AND A
			ticks += andReg(registers.A);
			break;
		case 0xA8: // XOR B 
			ticks += xorReg(registers.B);
			break;
		case 0xA9: // XOR C
			ticks += xorReg(registers.C);
			break;
		case 0xAA: // XOR D
			ticks += xorReg(registers.D);
			break;
		case 0xAB: // XOR E
			ticks += xorReg(registers.E);
			break;
		case 0xAC: // XOR H
			ticks += xorReg(registers.H);
			break;
		case 0xAD: // XOR L
			ticks += xorReg(registers.L);
			break;
		case 0xAE: // XOR (HL)
			ticks += xorHLInd(); 
			break;
		case 0xAF: // XOR A
			ticks += xorReg(registers.A);
			break;
		case 0xB0: // OR B
			ticks += orReg(registers.B);
			break;
		case 0xB1: // OR C
			ticks += orReg(registers.C);
			break;
		case 0xB2: // OR D
			ticks += orReg(registers.D);
			break;
		case 0xB3: // OR E
			ticks += orReg(registers.E);
			break;
		case 0xB4: // OR H
			ticks += orReg(registers.H);
			break;
		case 0xB5: // OR L
			ticks += orReg(registers.L);
			break;
		case 0xB6: // OR (HL)
			ticks += orHLInd(); 
			break;
		case 0xB7: // OR A
			ticks += orReg(registers.A);
			break;
		case 0xB8: // CP B
			ticks += compareReg(registers.B);
			break;
		case 0xB9: // CP C
			ticks += compareReg(registers.C);
			break;
		case 0xBA: // CP D
			ticks += compareReg(registers.D);
			break;
		case 0xBB: // CP E
			ticks += compareReg(registers.E);
			break;
		case 0xBC: // CP H
			ticks += compareReg(registers.H);
			break;
		case 0xBD: // CP L
			ticks += compareReg(registers.L);
			break;
		case 0xBE: // CP (HL)
			ticks += compareHLInd(); 
			break;
		case 0xBF: // CP A
			ticks += compareReg(registers.A);
			break;
		case 0xC0: // RET NZ
			ticks += conditionalReturnPC(flags.Z == 0); 
			break;
		case 0xC1: // POP BC
			ticks += popFromStack(&registers.B, &registers.C);
			break;
		case 0xC2: // JP NZ, nn
			ticks += conditionalJump(flags.Z == 0); 
			break;
		case 0xC3: // JP nn
			ticks += jump();
			break;
		case 0xC4: // CALL NZ, nn
			ticks += conditionalCall(flags.Z == 0); 
			break;
		case 0xC5: // PUSH BC
			ticks += pushToStack(registers.B, registers.C);
			break;
		case 0xC6: // ADD A, n
			ticks += addImm(); 
			break;
		case 0xC7: // RST 0
			ticks += sysCall(0x00); 
			break;
		case 0xC8: // RET Z
			ticks += conditionalReturnPC(flags.Z == 1);
			break;
		case 0xC9: // RET
			ticks += returnPC();
			break;
		case 0xCA: // JP Z, nn
			ticks += conditionalJump(flags.Z == 1);
			break;
		case 0xCB: // Prefix
			return cbPrefix();
			break;
		case 0xCC: // CALL Z, nn
			ticks += conditionalCall(flags.Z == 1);
			break;
		case 0xCD: // CALL nn
			ticks += call(); 
			break;
		case 0xCE: // ADC A, n
			ticks += adcImm();
			break;
		case 0xCF: // RST 8
			ticks += sysCall(0x08);
			break;
		case 0xD0: // RET NC
			ticks += conditionalReturnPC(flags.C == 0);
			break;
		case 0xD1: // POP DE
			ticks += popFromStack(&registers.D, &registers.E);
			break;
		case 0xD2: // JP NC, nn
			ticks += conditionalJump(flags.C == 0);
			break;
		case 0xD4: // CALL NC, nn
			ticks += conditionalCall(flags.C == 0); 
			break;
		case 0xD5: // PUSH DE
			ticks += pushToStack(registers.D, registers.E);
			break;
		case 0xD6: // SUB n
			ticks += subImm();
			break;
		case 0xD7: // RST 10H
			ticks += sysCall(0x10);
			break;
		case 0xD8: // RET C
			ticks += conditionalReturnPC(flags.C == 0); 
			break;
		case 0xD9: // RETI
			ticks += returnPCI();
			break;
		case 0xDA: // JP C, nn
			ticks += conditionalJump(flags.C == 0); 
			break;
		case 0xDC: // CALL C, nn
			ticks += conditionalCall(flags.C == 0);
			break;
		case 0xDE: // SBC A, n
			ticks += sbcImm(); 
			break;
		case 0xDF: // RST 18H
			ticks += sysCall(0x18); 
			break;
		case 0xE0: // LD (FF00+n), A
			ticks += writeIOPortN();
			break;
		case 0xE1: // POP HL
			ticks += popFromStack(&registers.H, &registers.L);
			break;
		case 0xE2: // LD (FF00+C), A
			ticks += writeIOPortC();
			break;
		case 0xE5: // PUSH HL
			ticks += pushToStack(registers.H, registers.L); 
			break;
		case 0xE6: // AND n
			ticks += andImm();
			break;
		case 0xE7: // RST 20H
			ticks += sysCall(0x20);
			break;
		case 0xE8: // ADD SP, dd
			ticks += addToSP();
			break;
		case 0xE9: // JP (HL)
			ticks += jumpHL(); 
			break;
		case 0xEA: // LD (nn), A
			ticks += storeAInd(); 
			break;
		case 0xEE: // XOR n
			ticks += xorImm(); 
			break;
		case 0xEF: // RST 28H
			ticks += sysCall(0x28);
			break;
		case 0xF0: // LD A, (FF00+n)
			ticks += readIOPortN();
			break;
		case 0xF1: // POP AF
			ticks += popFromStack(&registers.A, &registerF);
			setFRegister(registerF);
			break;
		case 0xF2: // LD A, (FF00+C)
			ticks += readIOPortC(); 
			break;
		case 0xF3: // DI
			ticks += disableInterrupts(); 
			break;
		case 0xF5: // PUSH AF
			registerF = getFRegister();
			ticks += pushToStack(registers.A, registerF);
			break;
		case 0xF6: // OR n
			ticks += orImm(); 
			break;
		case 0xF7: // RST 30H
			ticks += sysCall(0x30);
			break;
		case 0xF8: // LD HL, SP+dd
			ticks += addSPToHL();
			break;
		case 0xF9: // LD SP, HL
			ticks += loadHLToSP();
			break;
		case 0xFA: // LD A, (nn)
			ticks += loadAInd(); 
			break;
		case 0xFB: // EI
			ticks += enableInterrupts();
			break;
		case 0xFE: // CP n
			ticks += compareImm();
			break;
		case 0xFF: // RST 38H
			ticks += sysCall(0x38);
			break;
		default:
			printf("%x is an unsupported instruction\n", cpuInst); 
	}
	return ticks;
}

static int cbPrefix()
{

	// break the instruction down
	int inst = memoryRead(registers.PC++);
	int highInst = (inst >> 4) & 0xF;
	int lowInst = inst & 0xF;

#ifdef DEBUG
	printCBInst(registers.PC - 1, inst);
#endif
	
	// value that is being used
	int8_t* reg = 0;

	// flag if (HL) is being used
	int isInd = 0;
	int HL = 0;
	int8_t hlMem = 0;
	int memWrite = 1;

	// find out which value is going to be used
	switch(lowInst)
	{
		case 0x0:
			reg = &registers.B;
			break;
		case 0x1:
			reg = &registers.C;
			break;
		case 0x2:
			reg = &registers.D;
			break;
		case 0x3:
			reg = &registers.E;
			break;
		case 0x4:
			reg = &registers.H;
			break;
		case 0x5:
			reg = &registers.L;
			break;
		case 0x6:
			// (HL) is being used, grab it from memory and set the flag
			isInd = 1;
			HL = registers.H << 8 | registers.L;
			hlMem = memoryRead(HL);
			reg = &hlMem;
			break;
		case 0x7:
			reg = &registers.A;
			break;
		case 0x8:
			reg = &registers.B;
			break;
		case 0x9:
			reg = &registers.C;
			break;
		case 0xA:
			reg = &registers.D;
			break;
		case 0xB:
			reg = &registers.E;
			break;
		case 0xC:
			reg = &registers.H;
			break;
		case 0xD:
			reg = &registers.L;
			break;
		case 0xE:
			HL = 1;
			break;
		case 0xF:
			reg = &registers.A;
			break;
	}
	
	int ticks = 0;
	switch(highInst)
	{
		case 0x0:
			if(lowInst < 8) // RLC
				ticks += rotateRegLeftC(reg);
			else            // RRC
				ticks += rotateRegRightC(reg);
			break;
		case 0x1:
			if(lowInst < 8) // RL
				ticks += rotateRegLeft(reg);
			else            // RR
			 	ticks += rotateRegRight(reg);
			break;
		case 0x2:
			if(lowInst < 8) // SLA
				ticks += shiftRegLeftA(reg);
			else            // SRA
				ticks += shiftRegRightA(reg);
			break;			
		case 0x3:
			if(lowInst < 8) // SWAP
				ticks += swapReg(reg);
			else            // SRL
				ticks += shiftRegRightL(reg);
			break;
		case 0x4:           // BIT
		case 0x5:
		case 0x6:
		case 0x7:
			if (lowInst < 8)
				ticks += testRegBit(reg, (highInst - 4) * 2);
			else
				ticks += testRegBit(reg, ((highInst - 4) * 2) + 1);
			// test bit does not need to write back to memory.
			memWrite = 0;
			break;
		case 0x8:            // RES
		case 0x9:
		case 0xA:
		case 0xB:
			if (lowInst < 8)
				ticks += resetRegBit(reg, (highInst - 8) * 2);
			else
				ticks += resetRegBit(reg, ((highInst - 8) * 2) + 1);
			break;
		case 0xC:             // SET
		case 0xD:
		case 0xE:
		case 0xF:
			if (lowInst < 8)
				ticks += resetRegBit(reg, (highInst - 0xC) * 2);
			else
				ticks += resetRegBit(reg, ((highInst - 0xC) * 2) + 1);
			break;
	}
	
	// (HL) was used
	if(isInd)
	{
		// add time needed to read from memory
		ticks += 4;
		// check if we need to write back to memory.
		if(memWrite)
		{
			memoryWrite(HL, hlMem);
			// add time to write to memory 
			ticks += 4;
		}
	}

	return ticks;
}

static int getFRegister()
{
	// F register: ZNHC0000
	return (flags.Z << 7 | flags.N << 6 | flags.H << 5 | flags.C << 4) & 0xF0;
}

static void setFRegister(int8_t value)
{
	flags.Z = (value >> 7) & 0x1;
	flags.N = (value >> 6) & 0x1;
	flags.H = (value >> 5) & 0x1;
	flags.C = (value >> 4) & 0x1;
}
