#include "memory.h"
#include "registers.h"
#include "cpu_z80_inst.h"

int loadReg8(int8_t* src, int8_t* dest)
{
	*dest = *src;
	return 4;
}

int loadImmReg8(int8_t* reg)
{
	*reg = memoryRead(registers.PC++);
	return 8;
}

int loadImmReg16(uint8_t* reg1, uint8_t* reg2)
{
	*reg2 = memoryRead(registers.PC++);
	*reg1 = memoryRead(registers.PC++);
	return 12;	
}

int loadReg8Ind(uint8_t* reg, uint8_t* src1, uint8_t* src2)
{
	uint16_t memAddr = (*src1 << 8) | (*src2 & 0x00FF);
	*reg = memoryRead(memAddr);
	return 8;
}

int loadReg8HL(int8_t* reg)
{
	int memAddr = registers.H << 8 | registers.L;
	int8_t ind = memoryRead(memAddr);
	*reg = ind;
	return 8;
}

int storeReg8HL(int8_t reg)
{
	int memAddr = registers.H << 8 | registers.L;
	memoryWrite(memAddr, reg);
	return 8;
}

int storeHLImm()
{
	int memAddr = registers.H << 8 | registers.L;
	int imm = memoryRead(registers.PC++);
	memoryWrite(memAddr, imm);
	return 12;
}

int loadA(int8_t reg1, int8_t reg2)
{
	int memAddr = reg1 << 8 | reg2;
	registers.A = memoryRead(memAddr);
	return 8;
}

int loadAInd()
{
	int low = memoryRead(registers.PC++);
	int high = memoryRead(registers.PC++);
	loadA(high, low);
	return 16;
}

int storeA(int8_t reg1, int8_t reg2)
{
	int memAddr = reg1 << 8 | reg2;
	memoryWrite(memAddr, registers.A);
	return 8;
}

int storeAInd()
{
	int low = memoryRead(registers.PC++);
	int high = memoryRead(registers.PC++);
	storeA(high, low);
	return 16;
}

int readIOPortN()
{
	int n = memoryRead(registers.PC++);
	int memAddr = 0xFF00 + n;
	registers.A = memoryRead(memAddr);
	return 12;
}

int writeIOPortN()
{
	int n = memoryRead(registers.PC++);
	int memAddr = 0xFF00 + n;
	memoryWrite(memAddr, registers.A);
	return 12;
}

int readIOPortC()
{
	int memAddr = 0xFF00 + flags.C;
	registers.A = memoryRead(memAddr);
	return 8;
}

int writeIOPortC()
{
	int memAddr = 0xFF00 + flags.C;
	memoryWrite(memAddr, registers.A);
	return 8;
}

int storeIncrement()
{
	int HL = registers.H << 8 | registers.L;
	memoryWrite(HL, registers.A);
	HL++;
	registers.H = (HL >> 8) & 0xFF;
	registers.L = HL & 0xFF;
	return 8;
}

int loadIncrement()
{
	int HL = registers.H << 8 | registers.L;
	registers.A = memoryRead(HL);
	HL++;
	registers.H = (HL >> 8) & 0xFF;
	registers.L = HL & 0xFF;
	return 8;
}

int storeDecrement()
{
	int HL = registers.H << 8 | registers.L;
	memoryWrite(HL, registers.A);
	HL--;
	registers.H = (HL >> 8) & 0xFF;
	registers.L = HL & 0xFF;
	return 8;
}

int loadDecrement()
{
	int HL = registers.H << 8 | registers.L;
	registers.A = memoryRead(HL);
	HL--;
	registers.H = (HL >> 8) & 0xFF;
	registers.L = HL & 0xFF;
	return 8;
}

int loadReg16(int8_t* reg1, int8_t* reg2)
{
	*reg2 = memoryRead(registers.PC++);
	*reg1 = memoryRead(registers.PC++);
	return 12;
}

int loadSPImm()
{
	int low = memoryRead(registers.PC++);
	int high = memoryRead(registers.PC++);
	registers.SP = high << 8 | low;
	return 12;
}

int loadHLToSP()
{
	int HL = (registers.H << 8) | registers.L;
	registers.SP = HL;
	return 8;
}

int pushToStack(int8_t reg1, int8_t reg2)
{
	registers.SP--;
	memoryWrite(registers.SP, reg1);
	registers.SP--;
	memoryWrite(registers.SP, reg2);
	return 16;
}

int popFromStack(int8_t* reg1, int8_t* reg2)
{
	*reg2 = memoryRead(registers.SP);
	registers.SP++;
	*reg1 = memoryRead(registers.SP);
	registers.SP++;
	return 16;
}

static int add8SetFlags(int op1, int op2)
{
	int result = op1 + op2;
	flags.C = (result > 255 || result < 0);
	flags.Z = (result == 0);
	flags.H = (op1 & 0xF) + (op2 + 0xF) > 0xF;
	flags.N = 0;
	return result;
}

int addReg(int8_t reg)
{
	registers.A = add8SetFlags(registers.A, reg);
	return 4;
}

int addImm()
{
	int8_t imm = memoryRead(registers.PC++);
	registers.A = add8SetFlags(registers.A, imm);
	return 8;
}

int addHLInd()
{
	int memAddr = registers.H << 8 | registers.L;
	int8_t ind = memoryRead(memAddr);
	registers.A = add8SetFlags(registers.A, ind);
	return 8;
}

int adcReg(int8_t reg)
{
	registers.A = add8SetFlags(registers.A, reg + flags.C);
	return 4;
}

int adcImm()
{
	int8_t imm = memoryRead(registers.PC++);
	registers.A = add8SetFlags(registers.A, imm + flags.C);
	return 8;
}

int adcHLInd()
{
	int memAddr = registers.H << 8 | registers.L;
	int8_t ind = memoryRead(memAddr);
	registers.A = add8SetFlags(registers.A, ind + flags.C);
	return 8;
}

static int sub8SetFlags(int op1, int op2)
{
	int result = op1 - op2;
	flags.C = (result > 255 || result < 0);
	flags.Z = (result == 0);
	flags.H = (op1 & 0xF) < (op2 & 0xF);
	flags.N = 1;
	return result;
}

int subReg(int8_t reg)
{
	registers.A = sub8SetFlags(registers.A, reg);
	return 4;
}

int subImm()
{
	int imm = memoryRead(registers.PC++);
	registers.A = sub8SetFlags(registers.A, imm);
	return 8;
}

int subHLInd()
{
	int memAddr = registers.H << 8 | registers.L;
	int8_t ind = memoryRead(memAddr);
	registers.A = sub8SetFlags(registers.A, ind);
	return 8;
}

int sbcReg(int8_t reg)
{
	registers.A = sub8SetFlags(registers.A, reg + flags.C);
	return 4;
}

int sbcImm()
{
	int imm = memoryRead(registers.PC++);
	registers.A = sub8SetFlags(registers.A, imm + flags.C);
	return 8;
}

int sbcHLInd()
{
	int memAddr = registers.H << 8 | registers.L;
	int8_t ind = memoryRead(memAddr);
	registers.A = sub8SetFlags(registers.A, ind + flags.C);
	return 8;
}

static int and8SetFlags(int op1, int op2)
{
	int result = (op1 & op2) & 0xFF;
	flags.C = 0;
	flags.H = 1;
	flags.Z = (result == 0);
	flags.N = 0;
	return result;
}

int andReg(int8_t reg)
{
	registers.A = and8SetFlags(registers.A, reg);
	return 4;
}

int andImm()
{
	int imm = memoryRead(registers.PC++);
	registers.A = and8SetFlags(registers.A, imm);
	return 8;
}

int andHLInd()
{
	int memAddr = registers.H << 8 | registers.L;
	int8_t ind = memoryRead(memAddr);
	registers.A = and8SetFlags(registers.A, ind);
	return 8;
}

static int xor8SetFlags(int op1, int op2)
{
	int result = (op1 ^ op2) & 0xFF;
	flags.Z = (result == 0);
	flags.C = 0;
	flags.H = 0;
	flags.N = 0;
	return result;
}

int xorReg(int8_t reg)
{
	registers.A = xor8SetFlags(registers.A, reg);
	return 4;
}

int xorImm()
{
	int imm = memoryRead(registers.PC++);
	registers.A = xor8SetFlags(registers.A, imm);
	return 8;
}

int xorHLInd()
{
	int memAddr = registers.H << 8 | registers.L;
	int8_t ind = memoryRead(memAddr);
	registers.A = xor8SetFlags(registers.A, ind);
	return 8;
}

static int or8SetFlags(int op1, int op2)
{
	int result = (op1 | op2) & 0xFF;
	flags.Z = (result == 0);
	flags.C = 0;
	flags.H = 0;
	flags.N = 0;
	return result;
}

int orReg(int8_t reg)
{
	registers.A = or8SetFlags(registers.A, reg);
	return 4;
}

int orImm()
{
	int imm = memoryRead(registers.PC++);
	registers.A = or8SetFlags(registers.A, imm);
	return 8;
}

int orHLInd()
{
	int memAddr = registers.H << 8 | registers.L;
	int8_t ind = memoryRead(memAddr);
	registers.A = or8SetFlags(registers.A, ind);
	return 8;
}

int compareReg(int8_t reg)
{
	sub8SetFlags(registers.A, reg);
	return 4;
}

int compareImm()
{
	int imm = memoryRead(registers.PC++);
	sub8SetFlags(registers.A, imm);
	return 8;
}

int compareHLInd()
{
	int memAddr = registers.H << 8 | registers.L;
	int8_t ind = memoryRead(memAddr);
	sub8SetFlags(registers.A, ind);
	return 8;
}

int incReg8(int8_t* reg)
{
	flags.H = (*reg & 0xF) + 1 > 0xF;
	*reg += 1;
	flags.Z = (*reg == 0);
	flags.N = 0;
	return 4;
}

int incHLInd()
{
	int memAddr = registers.H << 8 | registers.L;
	int8_t ind = memoryRead(memAddr);
	flags.H = (ind & 0xF) + 1 > 0xF;
	ind += 1;
	flags.Z = (ind == 0);
	flags.N = 0;
	memoryWrite(memAddr, ind);
	return 12;
}

int decReg8(int8_t* reg)
{
	flags.H = (*reg & 0xF) < 1;
	*reg -= 1;
	flags.Z = (*reg == 0);
	flags.N = 1;
	return 4;
}

int decHLInd()
{
	int memAddr = registers.H << 8 | registers.L;
	int8_t ind = memoryRead(memAddr);
	flags.H = (ind & 0xF) < 1;
	ind -= 1;
	flags.Z = (ind == 0);
	flags.N = 1;
	memoryWrite(memAddr, ind);
	return 12;
}

int decimalyAdjustA()
{
	int high = (registers.A >> 4) & 0xF;
	int low = registers.A & 0xF;
	if(flags.N)
	{
		if(flags.C)
		{
			if(flags.H)
			{
				if((high >= 6) && (low >= 6))
					registers.A += 0x9A;
			}
			else if(high >= 7 && low <= 9)
			{
				registers.A += 0xA0;
			}
		}
		else
		{
			if(flags.H && (high <= 8) && (low >= 6))
			{
				registers.A += 0xFA;
			}
		}
	}
	else
	{
		if(flags.C)
		{
			if(flags.H)
			{
				if((high <= 3) && (low <= 3))
					registers.A += 0x66;
			}
			else if(high <= 2)
			{
				if(low <= 9)
				{
					registers.A += 0x60;
				}
				else
				{
					registers.A += 0x66;
				}
			}
		}
		else
		{
			if(flags.H)
			{
				if(low <= 3)
				{
					if((high <= 9))
					{
						registers.A += 0x06;
					}
					else
					{
						registers.A += 0x66;
					}
				}
			}
			else
			{
				if(low <= 9)
				{
					if(high > 9)
					{
						registers.A += 0x60;
						flags.C = 1;
					}
				}
				else 
				{
					if(high <= 8)
					{
						registers.A += 0x06;
					}
					else
					{
						registers.A += 0x66;
						flags.C = 1;
					}
				}
			}
		}
	}
	flags.Z = (registers.A == 0);
	return 4;
}

int complementA()
{
	registers.A ^= 0xFF;
	flags.N = 1;
	flags.H = 1;
	return 4;
}

static int add16SetFlags(int op1, int op2)
{
	int result = op1 + op2;
	flags.N = 0;
	flags.C = (result > 0xFFFF);
	flags.H = (op1 & 0xFFF) + (op2 + 0xFFF) > 0xFFF;
	return result;
}

int addReg16(int8_t reg1, int8_t reg2)
{
	int16_t reg16 = (reg1 << 8 | reg2) & 0xFFFF;
	int16_t HL = ((registers.H << 8) | registers.L) & 0xFFFF;
	HL = add16SetFlags(HL, reg16);
	registers.H = (HL >> 8) & 0xFF;
	registers.L = HL & 0xFF;
	return 8;
}

int incReg16(int8_t* reg1, int8_t* reg2)
{
	int regPair = (((*reg1 << 8) | *reg2) & 0xFFFF) + 1;
	*reg1 = (regPair >> 8) & 0xFF;
	*reg2 = regPair & 0xFF;
	return 8;
}

int decReg16(int8_t* reg1, int8_t* reg2)
{
	int regPair = (((*reg1 << 8) | *reg2) & 0xFFFF) - 1;
	*reg1 = (regPair >> 8) & 0xFF;
	*reg2 = regPair & 0xFF;
	return 8;
}

int addToSP()
{
	int8_t imm = memoryRead(registers.PC++);
	int result = imm + registers.SP;
	flags.Z = 0;
	flags.N = 0;
	flags.H = ((registers.SP & 0xFFF) + imm > 0xFFF); 
	flags.C = (result > 0xFFFF);
	registers.SP = result;
	return 16;
}

int addSPToHL()
{
	int HL = (registers.H << 8 | registers.L) & 0xFFFF;
	int8_t imm = memoryRead(registers.PC++);
	HL = registers.SP + imm;
	flags.Z = 0;
	flags.N = 0;
	flags.H = ((registers.SP & 0xFFF) + imm > 0xFFF);
	flags.C = (HL > 0xFFFF);
	registers.H = (HL >> 8) & 0xFF;
	registers.L = HL & 0xFF;
	return 12;
}

int rotateALeftC()
{
	int sevenBit = (registers.A >> 7) & 0x1;
	registers.A = (registers.A << 1) | sevenBit;
	flags.C = sevenBit;
	flags.N = 0;
	flags.H = 0;
	return 4;
}

int rotateALeft()
{
	int sevenBit = (registers.A >> 7) & 0x1;
	registers.A = (registers.A << 1) | flags.C;
	flags.C = sevenBit;
	flags.N = 0;
	flags.H = 0;
	return 4;
}

int rotateARightC()
{
	int zeroBit = registers.A & 0x1;
	registers.A = (registers.A >> 1) | (zeroBit << 7);
	flags.C = zeroBit;
	flags.N = 0;
	flags.H = 0;
	return 4;
}

int rotateARight()
{
	int zeroBit = registers.A & 0x1;
	registers.A = (registers.A >> 1) | (flags.C << 7);
	flags.C = zeroBit;
	flags.N = 0;
	flags.H = 0;
	return 4;
}

int rotateRegLeftC(int8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = (*reg << 1) | sevenBit;
	flags.C = sevenBit;
	flags.Z = (*reg == 0);
	flags.N = 0;
	flags.H = 0;
	return 8;
}

int rotateRegLeft(int8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = (*reg << 1) | flags.C;
	flags.C = sevenBit;
	flags.Z = (*reg == 0);
	flags.N = 0;
	flags.H = 0;
	return 8;
}

int rotateRegRightC(int8_t* reg)
{
	int zeroBit = *reg & 0x1;
	*reg = (*reg >> 1) | (zeroBit << 7);
	flags.C = zeroBit;
	flags.Z = (*reg == 0);
	flags.N = 0;
	flags.H = 0;
	return 8;
}

int rotateRegRight(int8_t* reg)
{
	int zeroBit = *reg & 0x1;
	*reg = (*reg >> 1) | (flags.C << 7);
	flags.C = zeroBit;
	flags.Z = (*reg == 0);
	flags.N = 0;
	flags.H = 0;
	return 8;
}

int shiftRegLeftA(int8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = *reg << 1;
	flags.C = sevenBit;
	flags.Z = (*reg == 0);
	flags.N = 0;
	flags.H = 0;
	return 8;
}

int swapReg(int8_t* reg)
{
	*reg = ((*reg >> 4) & 0xF0) | (*reg << 4);
	flags.Z = (*reg == 0);
	flags.C = 0;
	flags.N = 0;
	flags.H = 0;
	return 8;
}

int shiftRegRightA(int8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = *reg >> 1;
	flags.C = sevenBit;
	flags.Z = (*reg == 0);
	flags.N = 0;
	flags.H = 0;
	return 8;
}

int shiftRegRightL(int8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = (*reg >> 1) & 0x7F;
	flags.C = sevenBit;
	flags.Z = (*reg == 0);
	flags.N = 0;
	flags.H = 0;
	return 8;
}

int testRegBit(int8_t* reg, int n)
{
	int bit = (*reg >> n) & 0x1;
	flags.Z = (bit == 0);
	flags.N = 0;
	flags.H = 1;
	return 8;
}

int setRegBit(int8_t* reg, int n)
{
	int newBit = 1 << n;
	*reg |= newBit;
	return 8;
}

int resetRegBit(int8_t* reg, int n)
{
	int newBit = (1 << n) ^ 0xFF;
	*reg &= newBit;
	return 8;
}

int complementCarry()
{
	flags.C = flags.C ^ 1;
	flags.N = 0;
	flags.H = 0;
	return 4;
}

int setCarry()
{
	flags.C = 1;
	flags.N = 0;
	flags.H = 0;
	return 4;
}

int halt()
{
	// TODO implement halt
	return 4;
}

int stop()
{
	// TODO implement stop
	return -1;
}

int disableInterrupts()
{
	// TODO implement di
	return 4;
}

int enableInterrupts()
{
	// TODO implement ei
	return 4;
}

int jump()
{
	int8_t jumpLow = memoryRead(registers.PC++);
	int8_t jumpHigh = memoryRead(registers.PC++);
	int jumpAddr = (jumpHigh << 8) | jumpLow;
	registers.PC = jumpAddr;
	return 16;
}

int jumpHL()
{
	int HL = (registers.H << 8) | registers.L;
	registers.PC = HL;
	return 4;
}

int conditionalJump(int cond)
{
	if(cond)
	{
		return jump();		
	}
	registers.PC += 2;
	return 12; 
}

int relativeJump()
{
	int16_t jumpOffset = memoryRead(registers.PC++);
	registers.PC += jumpOffset;
	return 12;
}

int conditionalRelativeJump(int cond)
{
	if(cond)
	{
		return relativeJump();
	}
	registers.PC++;
	return 8;
}

int call()
{
	int pcHigh = (registers.PC >> 8) & 0xF;
	int pcLow = registers.PC & 0xF;
	pushToStack(pcLow, pcHigh);
	jump();
	return 24;
}

int conditionalCall(int cond)
{
	if(cond)
	{
		return call();
	}
	registers.PC += 2;
	return 12;
}

int returnPC()
{
	int8_t pcHigh, pcLow;
	popFromStack(&pcLow, &pcHigh);
	registers.PC = (pcHigh << 8) | pcLow;
	return 16;
}

int conditionalReturnPC(int cond)
{
	if(cond)
	{
		returnPC();
		return 20;
	}
	return 8;
}

int returnPCI()
{
	enableInterrupts();
	return returnPC();
}

int sysCall(int address)
{
	registers.PC = address;
	return 16;
}
