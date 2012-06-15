#include "cpu_z80_inst.h"

static struct z80Reg *registers;
static struct z80Flags *flags;
static mem_read memoryRead;
static mem_write memoryWrite;

void initialize_micro_ops(struct z80Reg *z80Registers, struct z80Flags *z80Flags,
                          mem_read read, mem_write write)
{
    registers = z80Registers;
    flags = z80Flags;
    memoryRead = read;
    memoryWrite = write;
}


/****************************
 *  8-bit load commands     * 
 ****************************/

int loadReg8(uint8_t* src, uint8_t* dest)
{
	*dest = *src;
	return 4;
}

int loadImmReg8(uint8_t* reg)
{
	*reg = memoryRead(registers->PC++);
	return 8;
}

int loadReg8HL(uint8_t* reg)
{
	int memAddr = ((registers->H & 0xFF) << 8 | (registers->L & 0xFF));
	uint8_t ind = memoryRead(memAddr);
	*reg = ind;
	return 8;
}

int storeReg8HL(uint8_t reg)
{
	int memAddr = ((registers->H & 0xFF) << 8 | (registers->L & 0xFF));
	memoryWrite(memAddr, reg);
	return 8;
}

int storeHLImm()
{
	int memAddr = ((registers->H & 0xFF) << 8 | (registers->L & 0xFF));
	int imm = memoryRead(registers->PC++);
	memoryWrite(memAddr, imm);
	return 12;
}

int loadA(uint8_t reg1, uint8_t reg2)
{
	int memAddr = (reg1 & 0xFF) << 8 | (reg2 & 0xFF);
	registers->A = memoryRead(memAddr);
	return 8;
}

int loadAInd()
{
	int low = memoryRead(registers->PC++);
	int high = memoryRead(registers->PC++);
	loadA(high, low);
	return 16;
}

int storeA(uint8_t reg1, uint8_t reg2)
{
	int memAddr = (reg1 & 0xFF) << 8 | (reg2 & 0xFF);
	memoryWrite(memAddr, registers->A);
	return 8;
}

int storeAInd()
{
	uint8_t low = memoryRead(registers->PC++);
	uint8_t high = memoryRead(registers->PC++);
	storeA(high, low);
	return 16;
}

int readIOPortN()
{
	int n = memoryRead(registers->PC++);
	int memAddr = 0xFF00 + n;
	registers->A = memoryRead(memAddr);
	return 12;
}

int writeIOPortN()
{
	int n = memoryRead(registers->PC++);
	int memAddr = 0xFF00 + n;
	memoryWrite(memAddr, registers->A);
	return 12;
}

int readIOPortC()
{
	int memAddr = 0xFF00 + (registers->C & 0xFF);
	registers->A = memoryRead(memAddr);
	return 8;
}

int writeIOPortC()
{
	int memAddr = 0xFF00 + (registers->C & 0xFF);
	memoryWrite(memAddr, registers->A);
	return 8;
}

int storeIncrement()
{
	int HL = ((registers->H & 0xFF) << 8 | (registers->L & 0xFF));
	memoryWrite(HL, registers->A);
	HL++;
	registers->H = (HL >> 8) & 0xFF;
	registers->L = HL & 0xFF;
	return 8;
}

int loadIncrement()
{
	int HL = ((registers->H & 0xFF) << 8 | (registers->L & 0xFF));
	registers->A = memoryRead(HL);
	HL++;
	registers->H = (HL >> 8) & 0xFF;
	registers->L = HL & 0xFF;
	return 8;
}

int storeDecrement()
{
	int HL = ((registers->H & 0xFF) << 8 | (registers->L & 0xFF));
	memoryWrite(HL, registers->A);
	HL--;
	registers->H = (HL >> 8) & 0xFF;
	registers->L = HL & 0xFF;
	return 8;
}

int loadDecrement()
{
	int HL = ((registers->H & 0xFF) << 8 | (registers->L & 0xFF));
	registers->A = memoryRead(HL);
	HL--;
	registers->H = (HL >> 8) & 0xFF;
	registers->L = HL & 0xFF;
	return 8;
}

/****************************
 *  16-bit load commands    * 
 ****************************/

int loadReg16(uint8_t* reg1, uint8_t* reg2)
{
	*reg2 = memoryRead(registers->PC++);
	*reg1 = memoryRead(registers->PC++);
	return 12;
}

int loadSPImm()
{
	int low = memoryRead(registers->PC++);
	int high = memoryRead(registers->PC++);
	registers->SP = high << 8 | low;
	return 12;
}

int loadHLToSP()
{
	int HL = (registers->H << 8) | (registers->L & 0xFF);
	registers->SP = HL;
	return 8;
}

int pushToStack(uint8_t reg1, uint8_t reg2)
{
	registers->SP--;
	memoryWrite(registers->SP, reg1);
	registers->SP--;
	memoryWrite(registers->SP, reg2);
	return 16;
}

int popFromStack(uint8_t* reg1, uint8_t* reg2)
{
	*reg2 = memoryRead(registers->SP);
	registers->SP++;
	*reg1 = memoryRead(registers->SP);
	registers->SP++;
	return 16;
}

/*****************************************
 *  8-bit arithmetic/logical commands    * 
 *****************************************/

static int add8SetFlags(uint8_t op1, uint8_t op2)
{
	int result = (op1 & 0xFF) + (op2 & 0xFF);
    flags->C = result > 255;
    flags->Z = (result & 0xFF) == 0;
	flags->H = (op1 & 0xF) + (op2 & 0xF) > 0xF;
	flags->N = 0;
	return result;
}

int addReg(uint8_t reg)
{
	registers->A = add8SetFlags(registers->A, reg);
	return 4;
}

int addImm()
{
	uint8_t imm = memoryRead(registers->PC++);
	registers->A = add8SetFlags(registers->A, imm);
	return 8;
}

int addHLInd()
{
	int memAddr = ((registers->H & 0xFF) << 8) | (registers->L & 0xFF);
	uint8_t ind = memoryRead(memAddr);
	registers->A = add8SetFlags(registers->A, ind);
	return 8;
}

int adcReg(uint8_t reg)
{
	registers->A = add8SetFlags(registers->A, reg + flags->C);
	return 4;
}

int adcImm()
{
	uint8_t imm = memoryRead(registers->PC++);
	registers->A = add8SetFlags(registers->A, imm + flags->C);
	return 8;
}

int adcHLInd()
{
	int memAddr = ((registers->H & 0xFF) << 8) | (registers->L & 0xFF);
	uint8_t ind = memoryRead(memAddr);
	registers->A = add8SetFlags(registers->A, ind + flags->C);
	return 8;
}

static int sub8SetFlags(uint8_t op1, uint8_t op2)
{
	int result = (op1 & 0xFF) - (op2 & 0xFF);
	flags->C = result < 0;
	flags->Z = ((result & 0xFF) == 0);
	flags->H = (op1 & 0xF) - (op2 & 0xF) < 0;
	flags->N = 1;
	return result;
}

int subReg(uint8_t reg)
{
	registers->A = sub8SetFlags(registers->A, reg);
	return 4;
}

int subImm()
{
	int imm = memoryRead(registers->PC++);
	registers->A = sub8SetFlags(registers->A, imm);
	return 8;
}

int subHLInd()
{
	int memAddr = ((registers->H & 0xFF) << 8) | (registers->L & 0xFF);
	uint8_t ind = memoryRead(memAddr);
	registers->A = sub8SetFlags(registers->A, ind);
	return 8;
}

int sbcReg(uint8_t reg)
{
	registers->A = sub8SetFlags(registers->A, reg + flags->C);
	return 4;
}

int sbcImm()
{
	int imm = memoryRead(registers->PC++);
	registers->A = sub8SetFlags(registers->A, imm + flags->C);
	return 8;
}

int sbcHLInd()
{
	int memAddr = registers->H << 8 | registers->L;
	uint8_t ind = memoryRead(memAddr);
	registers->A = sub8SetFlags(registers->A, ind + flags->C);
	return 8;
}

static uint8_t and8SetFlags(uint8_t op1, uint8_t op2)
{
	uint8_t result = (op1 & op2);
	flags->C = 0;
	flags->H = 1;
	flags->Z = (result == 0);
	flags->N = 0;
	return result;
}

int andReg(uint8_t reg)
{
	registers->A = and8SetFlags(registers->A, reg);
	return 4;
}

int andImm()
{
	int imm = memoryRead(registers->PC++);
	registers->A = and8SetFlags(registers->A, imm);
	return 8;
}

int andHLInd()
{
	int16_t memAddr = ((registers->H & 0xFF) << 8) | (registers->L & 0xFF);
	uint8_t ind = memoryRead(memAddr);
	registers->A = and8SetFlags(registers->A, ind);
	return 8;
}

static uint8_t xor8SetFlags(int op1, int op2)
{
	uint8_t result = (op1 ^ op2);
	flags->Z = (result == 0);
	flags->C = 0;
	flags->H = 0;
	flags->N = 0;
	return result;
}

int xorReg(uint8_t reg)
{
	registers->A = xor8SetFlags(registers->A, reg);
	return 4;
}

int xorImm()
{
	uint8_t imm = memoryRead(registers->PC++);
	registers->A = xor8SetFlags(registers->A, imm);
	return 8;
}

int xorHLInd()
{
	int16_t memAddr = ((registers->H & 0xFF) << 8) | (registers->L & 0xFF);
	uint8_t ind = memoryRead(memAddr);
	registers->A = xor8SetFlags(registers->A, ind);
	return 8;
}

static uint8_t or8SetFlags(int op1, int op2)
{
	uint8_t result = (op1 | op2);
	flags->Z = (result == 0);
	flags->C = 0;
	flags->H = 0;
	flags->N = 0;
	return result;
}

int orReg(uint8_t reg)
{
	registers->A = or8SetFlags(registers->A, reg);
	return 4;
}

int orImm()
{
	uint8_t imm = memoryRead(registers->PC++);
	registers->A = or8SetFlags(registers->A, imm);
	return 8;
}

int orHLInd()
{
	int16_t memAddr = ((registers->H & 0xFF) << 8) | (registers->L & 0xFF);
	uint8_t ind = memoryRead(memAddr);
	registers->A = or8SetFlags(registers->A, ind);
	return 8;
}

int compareReg(uint8_t reg)
{
	sub8SetFlags(registers->A, reg);
	return 4;
}

int compareImm()
{
	uint8_t imm = memoryRead(registers->PC++);
	sub8SetFlags(registers->A, imm);
	return 8;
}

int compareHLInd()
{
	int16_t memAddr = ((registers->H & 0xFF) << 8) | (registers->L & 0xFF);
	uint8_t ind = memoryRead(memAddr);
	sub8SetFlags(registers->A, ind);
	return 8;
}

int incReg8(uint8_t* reg)
{
	flags->H = (*reg & 0xF) + 1 > 0xF;
	*reg += 1;
	flags->Z = (*reg == 0);
	flags->N = 0;
	return 4;
}

int incHLInd()
{
	int16_t memAddr = ((registers->H & 0xFF) << 8) | (registers->L & 0xFF);
	uint8_t ind = memoryRead(memAddr);
	flags->H = (ind & 0xF) + 1 > 0xF;
	ind += 1;
	flags->Z = (ind == 0);
	flags->N = 0;
	memoryWrite(memAddr, ind);
	return 12;
}

int decReg8(uint8_t* reg)
{
	flags->H = (*reg & 0xF) - 1 < 0;
	*reg -= 1;
	flags->Z = (*reg == 0);
	flags->N = 1;
	return 4;
}

int decHLInd()
{
	int16_t memAddr = ((registers->H & 0xFF) << 8) | (registers->L & 0xFF);
	uint8_t ind = memoryRead(memAddr);
	flags->H = (ind & 0xF) < 1;
	ind -= 1;
	flags->Z = (ind == 0);
	flags->N = 1;
	memoryWrite(memAddr, ind);
	return 12;
}

int decimalyAdjustA()
{
	int high = (registers->A >> 4) & 0xF;
	int low = registers->A & 0xF;
	if(flags->N)
	{
		if(flags->C)
		{
			if(flags->H)
			{
				if((high >= 6) && (low >= 6))
					registers->A += 0x9A;
			}
			else if(high >= 7 && low <= 9)
			{
				registers->A += 0xA0;
			}
		}
		else
		{
			if(flags->H && (high <= 8) && (low >= 6))
			{
				registers->A += 0xFA;
			}
		}
	}
	else
	{
		if(flags->C)
		{
			if(flags->H)
			{
				if((high <= 3) && (low <= 3))
					registers->A += 0x66;
			}
			else if(high <= 2)
			{
				if(low <= 9)
				{
					registers->A += 0x60;
				}
				else
				{
					registers->A += 0x66;
				}
			}
		}
		else
		{
			if(flags->H)
			{
				if(low <= 3)
				{
					if((high <= 9))
					{
						registers->A += 0x06;
					}
					else
					{
						registers->A += 0x66;
					}
				}
			}
			else
			{
				if(low <= 9)
				{
					if(high > 9)
					{
						registers->A += 0x60;
						flags->C = 1;
					}
				}
				else 
				{
					if(high <= 8)
					{
						registers->A += 0x06;
					}
					else
					{
						registers->A += 0x66;
						flags->C = 1;
					}
				}
			}
		}
	}
	flags->Z = (registers->A == 0);
	return 4;
}

int complementA()
{
	registers->A ^= 0xFF;
	flags->N = 1;
	flags->H = 1;
	return 4;
}

/**************************************
 * 16-bit arithmetic/logical commands * 
 **************************************/

static int16_t add16SetFlags(uint16_t op1, uint16_t op2)
{
	int result = (op1 & 0xFFFF) + (op2 & 0xFFFF);
	flags->N = 0;
	flags->C = (result > 0xFFFF);
	flags->H = (op1 & 0xFFF) + (op2 & 0xFFF) > 0xFFF;
	return result;
}

int addReg16(uint8_t reg1, uint8_t reg2)
{
	uint16_t reg16 = (reg1 << 8 | reg2) & 0xFFFF;
	uint16_t HL = ((registers->H & 0xFF) << 8) | (registers->L & 0xFF);
	HL = add16SetFlags(HL, reg16);
	registers->H = (HL >> 8) & 0xFF;
	registers->L = HL & 0xFF;
	return 8;
}

int incReg16(uint8_t* reg1, uint8_t* reg2)
{
	uint16_t regPair = ((*reg1 & 0xFF) << 8 | (*reg2 & 0xFF)) + 1;
	*reg1 = (regPair >> 8) & 0xFF;
	*reg2 = regPair & 0xFF;
	return 8;
}

int decReg16(uint8_t* reg1, uint8_t* reg2)
{
	int16_t regPair = ((*reg1 & 0xFF) << 8 | (*reg2 & 0xFF)) - 1;
	*reg1 = (regPair >> 8) & 0xFF;
	*reg2 = regPair & 0xFF;
	return 8;
}

int addToSP()
{
	uint8_t imm = memoryRead(registers->PC++);
	int result = (imm & 0xFF) + (registers->SP & 0xFFFF);
	flags->Z = 0;
	flags->N = 0;
	flags->H = ((registers->SP & 0xFFF) + imm > 0xFFF); 
	flags->C = (result > 0xFFFF);
	registers->SP = result;
	return 16;
}

int addSPToHL()
{
	int HL = ((registers->H & 0xFF) << 8) | (registers->L & 0xFF);
	uint8_t imm = memoryRead(registers->PC++);
	HL = (registers->SP & 0xFFFF) + (imm & 0xFF);
	flags->Z = 0;
	flags->N = 0;
	flags->H = ((registers->SP & 0xFFF) + imm > 0xFFF);
	flags->C = (HL > 0xFFFF);
	registers->H = (HL >> 8) & 0xFF;
	registers->L = HL & 0xFF;
	return 12;
}

/**************************************
 * Rotate and shift commands          * 
 **************************************/

int rotateALeftC()
{
	int sevenBit = (registers->A >> 7) & 0x1;
	registers->A = (registers->A << 1) | sevenBit;
	flags->C = sevenBit;
	flags->N = 0;
	flags->H = 0;
	return 4;
}

int rotateALeft()
{
	int sevenBit = (registers->A >> 7) & 0x1;
	registers->A = (registers->A << 1) | flags->C;
	flags->C = sevenBit;
	flags->N = 0;
	flags->H = 0;
	return 4;
}

int rotateARightC()
{
	int zeroBit = registers->A & 0x1;
	registers->A = (registers->A >> 1) | (zeroBit << 7);
	flags->C = zeroBit;
	flags->N = 0;
	flags->H = 0;
	return 4;
}

int rotateARight()
{
	int zeroBit = registers->A & 0x1;
	registers->A = (registers->A >> 1) | (flags->C << 7);
	flags->C = zeroBit;
	flags->N = 0;
	flags->H = 0;
	return 4;
}

int rotateRegLeftC(uint8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = (*reg << 1) | sevenBit;
	flags->C = sevenBit;
	flags->Z = (*reg == 0);
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int rotateRegLeft(uint8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = (*reg << 1) | flags->C;
	flags->C = sevenBit;
	flags->Z = (*reg == 0);
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int rotateRegRightC(uint8_t* reg)
{
	int zeroBit = *reg & 0x1;
	*reg = (*reg >> 1) | (zeroBit << 7);
	flags->C = zeroBit;
	flags->Z = (*reg == 0);
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int rotateRegRight(uint8_t* reg)
{
	int zeroBit = *reg & 0x1;
	*reg = (*reg >> 1) | (flags->C << 7);
	flags->C = zeroBit;
	flags->Z = (*reg == 0);
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int shiftRegLeftA(uint8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = *reg << 1;
	flags->C = sevenBit;
	flags->Z = (*reg == 0);
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int swapReg(uint8_t* reg)
{
	*reg = ((*reg >> 4) & 0xF0) | (*reg << 4);
	flags->Z = (*reg == 0);
	flags->C = 0;
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int shiftRegRightA(uint8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = *reg >> 1;
	flags->C = sevenBit;
	flags->Z = (*reg == 0);
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int shiftRegRightL(uint8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = (*reg >> 1) & 0x7F;
	flags->C = sevenBit;
	flags->Z = (*reg == 0);
	flags->N = 0;
	flags->H = 0;
	return 8;
}

/**************************************
 * Single bit operations              * 
 **************************************/

int testRegBit(uint8_t* reg, int n)
{
	int bit = (*reg >> n) & 0x1;
	flags->Z = (bit == 0);
	flags->N = 0;
	flags->H = 1;
	return 8;
}

int setRegBit(uint8_t* reg, int n)
{
	int newBit = 1 << n;
	*reg |= newBit;
	return 8;
}

int resetRegBit(uint8_t* reg, int n)
{
	int newBit = (1 << n) ^ 0xFF;
	*reg &= newBit;
	return 8;
}

/**************************************
 * CPU Control Commands               * 
 **************************************/

int complementCarry()
{
	flags->C = flags->C ^ 1;
	flags->N = 0;
	flags->H = 0;
	return 4;
}

int setCarry()
{
	flags->C = 1;
	flags->N = 0;
	flags->H = 0;
	return 4;
}

int halt()
{
	// TODO implement halt
	printf("halting\n");
	return 4;
}

int stop()
{
	// TODO implement stop
	exit(0);	
	return -1;
}

int disableInterrupts()
{
	// intMasEnable = 0;
	return 4;
}

int enableInterrupts()
{
	// intMasEnable = 1;
	return 4;
}

/**************************************
 * Jump Commands                      * 
 **************************************/

int jump()
{
	uint8_t jumpLow = memoryRead(registers->PC++);
	uint8_t jumpHigh = memoryRead(registers->PC++);
	int jumpAddr = ((jumpHigh & 0xFF) << 8) | (jumpLow & 0xFF);
	registers->PC = jumpAddr;
	return 16;
}

int jumpHL()
{
	int HL = (registers->H << 8) | registers->L;
	registers->PC = HL;
	return 4;
}

int conditionalJump(int cond)
{
	if(cond)
	{
		return jump();		
	}
	registers->PC += 2;
	return 12; 
}

int relativeJump()
{
	uint8_t jumpOffset = memoryRead(registers->PC++);
	registers->PC += jumpOffset;
	return 12;
}

int conditionalRelativeJump(int cond)
{
	if(cond)
	{
		return relativeJump();
	}
	registers->PC++;
	return 8;
}

int call()
{
	registers->PC+=2;
	int pcHigh = (registers->PC >> 8) & 0xFF;
	int pcLow = registers->PC & 0xFF;
	pushToStack(pcLow, pcHigh);
	registers->PC-=2;
	jump();
	return 24;
}

int conditionalCall(int cond)
{
	if(cond)
	{
		return call();
	}
	registers->PC += 2;
	return 12;
}

int returnPC()
{
	uint8_t pcHigh, pcLow;
	popFromStack(&pcLow, &pcHigh);
	registers->PC = ((pcHigh & 0xFF) << 8) | (pcLow & 0xFF);
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
	int pcHigh = (registers->PC >> 8) & 0xF;
	int pcLow = registers->PC & 0xF;
	pushToStack(pcLow, pcHigh);
	registers->PC = address;
	return 16;
}
