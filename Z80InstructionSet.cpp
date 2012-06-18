#include "Z80InstructionSet.h"

/****************************
 *  8-bit load commands     * 
 ****************************/

int Z80InstructionSet::loadReg8(uint8_t* src, uint8_t* dest)
{
	*dest = *src;
	return 4;
}

int Z80InstructionSet::loadImmReg8(uint8_t* reg)
{
	*reg = memory->memoryRead(registers->PC.val++);
	return 8;
}

int Z80InstructionSet::loadReg8HL(uint8_t* reg)
{
	uint16_t memAddr = registers->HL.val; 
	uint8_t ind = memory->memoryRead(memAddr);
	*reg = ind;
	return 8;
}

int Z80InstructionSet::storeReg8HL(uint8_t reg)
{
	uint16_t memAddr = registers->HL.val; 
	memory->memoryWrite(memAddr, reg);
	return 8;
}

int Z80InstructionSet::storeHLImm()
{
    uint16_t memAddr = registers->HL.val;
	uint8_t imm = memory->memoryRead(registers->PC.val++);
	memory->memoryWrite(memAddr, imm);
	return 12;
}

int Z80InstructionSet::loadA(RegisterPair regPair)
{
	uint16_t memAddr = regPair.val;
	registers->AF.hi = memory->memoryRead(memAddr);
	return 8;
}

int Z80InstructionSet::loadAInd()
{
	uint8_t low = memory->memoryRead(registers->PC.val++);
	uint8_t high = memory->memoryRead(registers->PC.val++);
    RegisterPair test(high, low);
	loadA(RegisterPair(high, low));
	return 16;
}

int Z80InstructionSet::storeA(RegisterPair regPair)
{
	uint16_t memAddr = regPair.val;
    memory->memoryWrite(memAddr, registers->AF.hi);
	return 8;
}

int Z80InstructionSet::storeAInd()
{
	uint8_t low = memory->memoryRead(registers->PC.val++);
	uint8_t high = memory->memoryRead(registers->PC.val++);
	storeA(RegisterPair(high, low));
	return 16;
}

int Z80InstructionSet::readIOPortN()
{
	uint8_t n = memory->memoryRead(registers->PC.val++);
	uint16_t memAddr = 0xFF00 + n;
	registers->AF.hi = memory->memoryRead(memAddr);
	return 12;
}

int Z80InstructionSet::writeIOPortN()
{
	uint8_t n = memory->memoryRead(registers->PC.val++);
	uint16_t memAddr = 0xFF00 + n;
	memory->memoryWrite(memAddr, registers->AF.hi);
	return 12;
}

int Z80InstructionSet::readIOPortC()
{
	uint16_t memAddr = 0xFF00 + registers->BC.lo;
	registers->AF.hi = memory->memoryRead(memAddr);
	return 8;
}

int Z80InstructionSet::writeIOPortC()
{
	uint16_t memAddr = 0xFF00 + registers->BC.lo;
	memory->memoryWrite(memAddr, registers->AF.hi);
	return 8;
}

int Z80InstructionSet::storeIncrement()
{
	memory->memoryWrite(registers->HL.val, registers->AF.hi);
	registers->HL.val++;
	return 8;
}

int Z80InstructionSet::loadIncrement()
{
	registers->AF.hi = memory->memoryRead(registers->HL.val);
	registers->HL.val++;
	return 8;
}

int Z80InstructionSet::storeDecrement()
{
	memory->memoryWrite(registers->HL.val, registers->AF.hi);
	registers->HL.val--;
	return 8;
}

int Z80InstructionSet::loadDecrement()
{
	registers->AF.hi = memory->memoryRead(registers->HL.val);
	registers->HL.val--;
	return 8;
}

/****************************
 *  16-bit load commands    * 
 ****************************/

int Z80InstructionSet::loadReg16(RegisterPair *regPair)
{
	regPair->lo = memory->memoryRead(registers->PC.val++);
	regPair->hi = memory->memoryRead(registers->PC.val++);
	return 12;
}

int Z80InstructionSet::loadSPImm()
{
	registers->SP.lo = memory->memoryRead(registers->PC.val++);
	registers->SP.hi = memory->memoryRead(registers->PC.val++);
	return 12;
}

int Z80InstructionSet::loadHLToSP()
{
	registers->SP.val = registers->HL.val;
	return 8;
}

int Z80InstructionSet::pushToStack(RegisterPair regPair)
{
	registers->SP.val--;
	memory->memoryWrite(registers->SP.val, regPair.lo);
	registers->SP.val--;
	memory->memoryWrite(registers->SP.val, regPair.hi);
	return 16;
}

int Z80InstructionSet::popFromStack(RegisterPair *regPair)
{
	regPair->hi = memory->memoryRead(registers->SP.val);
	registers->SP.val++;
	regPair->lo = memory->memoryRead(registers->SP.val);
	registers->SP.val++;
	return 16;
}

/*****************************************
 *  8-bit arithmetic/logical commands    * 
 *****************************************/

uint8_t Z80InstructionSet::add8SetFlags(uint8_t op1, uint8_t op2)
{
	int result = (op1 & 0xFF) + (op2 & 0xFF);
    flags->C = result > 255;
    flags->Z = (result & 0xFF) == 0;
	flags->H = (op1 & 0xF) + (op2 & 0xF) > 0xF;
	flags->N = 0;
	return result;
}

int Z80InstructionSet::addReg(uint8_t reg)
{
	registers->AF.hi = add8SetFlags(registers->AF.hi, reg);
	return 4;
}

int Z80InstructionSet::addImm()
{
	uint8_t imm = memory->memoryRead(registers->PC.val++);
	registers->AF.hi = add8SetFlags(registers->AF.hi, imm);
	return 8;
}

int Z80InstructionSet::addHLInd()
{
	uint16_t memAddr = registers->HL.val;
	uint8_t ind = memory->memoryRead(memAddr);
	registers->AF.hi = add8SetFlags(registers->AF.hi, ind);
	return 8;
}

int Z80InstructionSet::adcReg(uint8_t reg)
{
	registers->AF.hi = add8SetFlags(registers->AF.hi, reg + flags->C);
	return 4;
}

int Z80InstructionSet::adcImm()
{
	uint8_t imm = memory->memoryRead(registers->PC.val++);
	registers->AF.hi = add8SetFlags(registers->AF.hi, imm + flags->C);
	return 8;
}

int Z80InstructionSet::adcHLInd()
{
	uint16_t memAddr = registers->HL.val;
	uint8_t ind = memory->memoryRead(memAddr);
	registers->AF.hi = add8SetFlags(registers->AF.hi, ind + flags->C);
	return 8;
}

uint8_t Z80InstructionSet::sub8SetFlags(uint8_t op1, uint8_t op2)
{
	int result = (op1 & 0xFF) - (op2 & 0xFF);
	flags->C = result < 0;
	flags->Z = ((result & 0xFF) == 0);
	flags->H = (op1 & 0xF) - (op2 & 0xF) < 0;
	flags->N = 1;
	return result;
}

int Z80InstructionSet::subReg(uint8_t reg)
{
	registers->AF.hi = sub8SetFlags(registers->AF.hi, reg);
	return 4;
}

int Z80InstructionSet::subImm()
{
	int imm = memory->memoryRead(registers->PC.val++);
	registers->AF.hi = sub8SetFlags(registers->AF.hi, imm);
	return 8;
}

int Z80InstructionSet::subHLInd()
{
	uint16_t memAddr = registers->HL.val;
	uint8_t ind = memory->memoryRead(memAddr);
	registers->AF.hi = sub8SetFlags(registers->AF.hi, ind);
	return 8;
}

int Z80InstructionSet::sbcReg(uint8_t reg)
{
	registers->AF.hi = sub8SetFlags(registers->AF.hi, reg + flags->C);
	return 4;
}

int Z80InstructionSet::sbcImm()
{
	uint8_t imm = memory->memoryRead(registers->PC.val++);
	registers->AF.hi = sub8SetFlags(registers->AF.hi, imm + flags->C);
	return 8;
}

int Z80InstructionSet::sbcHLInd()
{
	uint16_t memAddr = registers->HL.val;
	uint8_t ind = memory->memoryRead(memAddr);
	registers->AF.hi = sub8SetFlags(registers->AF.hi, ind + flags->C);
	return 8;
}

uint8_t Z80InstructionSet::and8SetFlags(uint8_t op1, uint8_t op2)
{
	uint8_t result = (op1 & op2);
	flags->C = 0;
	flags->H = 1;
	flags->Z = (result == 0);
	flags->N = 0;
	return result;
}

int Z80InstructionSet::andReg(uint8_t reg)
{
	registers->AF.hi = and8SetFlags(registers->AF.hi, reg);
	return 4;
}

int Z80InstructionSet::andImm()
{
	uint8_t imm = memory->memoryRead(registers->PC.val++);
	registers->AF.hi = and8SetFlags(registers->AF.hi, imm);
	return 8;
}

int Z80InstructionSet::andHLInd()
{
	uint16_t memAddr = registers->HL.val;
	uint8_t ind = memory->memoryRead(memAddr);
	registers->AF.hi = and8SetFlags(registers->AF.hi, ind);
	return 8;
}

uint8_t Z80InstructionSet::xor8SetFlags(uint8_t op1, uint8_t op2)
{
	uint8_t result = (op1 ^ op2);
	flags->Z = (result == 0);
	flags->C = 0;
	flags->H = 0;
	flags->N = 0;
	return result;
}

int Z80InstructionSet::xorReg(uint8_t reg)
{
	registers->AF.hi = xor8SetFlags(registers->AF.hi, reg);
	return 4;
}

int Z80InstructionSet::xorImm()
{
	uint8_t imm = memory->memoryRead(registers->PC.val++);
	registers->AF.hi = xor8SetFlags(registers->AF.hi, imm);
	return 8;
}

int Z80InstructionSet::xorHLInd()
{
	int16_t memAddr = registers->HL.val;
	uint8_t ind = memory->memoryRead(memAddr);
	registers->AF.hi = xor8SetFlags(registers->AF.hi, ind);
	return 8;
}

uint8_t Z80InstructionSet::or8SetFlags(uint8_t op1, uint8_t op2)
{
	uint8_t result = (op1 | op2);
	flags->Z = (result == 0);
	flags->C = 0;
	flags->H = 0;
	flags->N = 0;
	return result;
}

int Z80InstructionSet::orReg(uint8_t reg)
{
	registers->AF.hi = or8SetFlags(registers->AF.hi, reg);
	return 4;
}

int Z80InstructionSet::orImm()
{
	uint8_t imm = memory->memoryRead(registers->PC.val++);
	registers->AF.hi = or8SetFlags(registers->AF.hi, imm);
	return 8;
}

int Z80InstructionSet::orHLInd()
{
	int16_t memAddr = registers->HL.val;
	uint8_t ind = memory->memoryRead(memAddr);
	registers->AF.hi = or8SetFlags(registers->AF.hi, ind);
	return 8;
}

int Z80InstructionSet::compareReg(uint8_t reg)
{
	sub8SetFlags(registers->AF.hi, reg);
	return 4;
}

int Z80InstructionSet::compareImm()
{
	uint8_t imm = memory->memoryRead(registers->PC.val++);
	sub8SetFlags(registers->AF.hi, imm);
	return 8;
}

int Z80InstructionSet::compareHLInd()
{
	int16_t memAddr = registers->HL.val;
	uint8_t ind = memory->memoryRead(memAddr);
	sub8SetFlags(registers->AF.hi, ind);
	return 8;
}

int Z80InstructionSet::incReg8(uint8_t* reg)
{
	flags->H = (*reg & 0xF) + 1 > 0xF;
	*reg += 1;
	flags->Z = (*reg == 0);
	flags->N = 0;
	return 4;
}

int Z80InstructionSet::incHLInd()
{
	int16_t memAddr = registers->HL.val;
	uint8_t ind = memory->memoryRead(memAddr);
	flags->H = (ind & 0xF) + 1 > 0xF;
	ind += 1;
	flags->Z = (ind == 0);
	flags->N = 0;
	memory->memoryWrite(memAddr, ind);
	return 12;
}

int Z80InstructionSet::decReg8(uint8_t* reg)
{
	flags->H = (*reg & 0xF) - 1 < 0;
	*reg -= 1;
	flags->Z = (*reg == 0);
	flags->N = 1;
	return 4;
}

int Z80InstructionSet::decHLInd()
{
	int16_t memAddr = registers->HL.val;
	uint8_t ind = memory->memoryRead(memAddr);
	flags->H = (ind & 0xF) < 1;
	ind -= 1;
	flags->Z = (ind == 0);
	flags->N = 1;
	memory->memoryWrite(memAddr, ind);
	return 12;
}

int Z80InstructionSet::decimalyAdjustA()
{
	int high = (registers->AF.hi >> 4) & 0xF;
	int low = registers->AF.hi & 0xF;
	if(flags->N)
	{
		if(flags->C)
		{
			if(flags->H)
			{
				if((high >= 6) && (low >= 6))
					registers->AF.hi += 0x9A;
			}
			else if(high >= 7 && low <= 9)
			{
				registers->AF.hi += 0xA0;
			}
		}
		else
		{
			if(flags->H && (high <= 8) && (low >= 6))
			{
				registers->AF.hi += 0xFA;
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
					registers->AF.hi += 0x66;
			}
			else if(high <= 2)
			{
				if(low <= 9)
				{
					registers->AF.hi += 0x60;
				}
				else
				{
					registers->AF.hi += 0x66;
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
						registers->AF.hi += 0x06;
					}
					else
					{
						registers->AF.hi += 0x66;
					}
				}
			}
			else
			{
				if(low <= 9)
				{
					if(high > 9)
					{
						registers->AF.hi += 0x60;
						flags->C = 1;
					}
				}
				else 
				{
					if(high <= 8)
					{
						registers->AF.hi += 0x06;
					}
					else
					{
						registers->AF.hi += 0x66;
						flags->C = 1;
					}
				}
			}
		}
	}
	flags->Z = (registers->AF.hi == 0);
	return 4;
}

int Z80InstructionSet::complementA()
{
	registers->AF.hi ^= 0xFF;
	flags->N = 1;
	flags->H = 1;
	return 4;
}

/**************************************
 * 16-bit arithmetic/logical commands * 
 **************************************/

uint16_t Z80InstructionSet::add16SetFlags(uint16_t op1, uint16_t op2)
{
	int result = (op1 & 0xFFFF) + (op2 & 0xFFFF);
	flags->N = 0;
	flags->C = (result > 0xFFFF);
	flags->H = (op1 & 0xFFF) + (op2 & 0xFFF) > 0xFFF;
	return result;
}

int Z80InstructionSet::addReg16(RegisterPair regPair)
{
	registers->HL.val = add16SetFlags(registers->HL.val, regPair.val);
	return 8;
}

int Z80InstructionSet::incReg16(RegisterPair *regPair)
{
	regPair->val++;
	return 8;
}

int Z80InstructionSet::decReg16(RegisterPair *regPair)
{
	regPair->val--;
    return 8;
}

uint16_t Z80InstructionSet::addToSp(int8_t value)
{
    int result = registers->SP.val + value;
	flags->Z = 0;
	flags->N = 0;
	flags->H = ((registers->SP.val & 0xFFF) + value > 0xFFF); 
	flags->C = (result > 0xFFFF);
	return result;
}

int Z80InstructionSet::addImmToSP()
{
	uint8_t imm = memory->memoryRead(registers->PC.val++);
    registers->SP.val = addToSp(imm);	
	return 16;
}

int Z80InstructionSet::addSPToHL()
{
	uint8_t imm = memory->memoryRead(registers->PC.val++);
	registers->SP.val = addToSp(imm);
	return 12;
}

/**************************************
 * Rotate and shift commands          * 
 **************************************/

int Z80InstructionSet::rotateALeftC()
{
	int sevenBit = (registers->AF.hi >> 7) & 0x1;
	registers->AF.hi = (registers->AF.hi << 1) | sevenBit;
	flags->C = sevenBit;
	flags->N = 0;
	flags->H = 0;
	return 4;
}

int Z80InstructionSet::rotateALeft()
{
	int sevenBit = (registers->AF.hi >> 7) & 0x1;
	registers->AF.hi = (registers->AF.hi << 1) | flags->C;
	flags->C = sevenBit;
	flags->N = 0;
	flags->H = 0;
	return 4;
}

int Z80InstructionSet::rotateARightC()
{
	int zeroBit = registers->AF.hi & 0x1;
	registers->AF.hi = (registers->AF.hi >> 1) | (zeroBit << 7);
	flags->C = zeroBit;
	flags->N = 0;
	flags->H = 0;
	return 4;
}

int Z80InstructionSet::rotateARight()
{
	int zeroBit = registers->AF.hi & 0x1;
	registers->AF.hi = (registers->AF.hi >> 1) | (flags->C << 7);
	flags->C = zeroBit;
	flags->N = 0;
	flags->H = 0;
	return 4;
}

int Z80InstructionSet::rotateRegLeftC(uint8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = (*reg << 1) | sevenBit;
	flags->C = sevenBit;
	flags->Z = (*reg == 0);
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int Z80InstructionSet::rotateRegLeft(uint8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = (*reg << 1) | flags->C;
	flags->C = sevenBit;
	flags->Z = (*reg == 0);
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int Z80InstructionSet::rotateRegRightC(uint8_t* reg)
{
	int zeroBit = *reg & 0x1;
	*reg = (*reg >> 1) | (zeroBit << 7);
	flags->C = zeroBit;
	flags->Z = (*reg == 0);
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int Z80InstructionSet::rotateRegRight(uint8_t* reg)
{
	int zeroBit = *reg & 0x1;
	*reg = (*reg >> 1) | (flags->C << 7);
	flags->C = zeroBit;
	flags->Z = (*reg == 0);
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int Z80InstructionSet::shiftRegLeftA(uint8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = *reg << 1;
	flags->C = sevenBit;
	flags->Z = (*reg == 0);
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int Z80InstructionSet::swapReg(uint8_t* reg)
{
	*reg = ((*reg >> 4) & 0xF0) | (*reg << 4);
	flags->Z = (*reg == 0);
	flags->C = 0;
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int Z80InstructionSet::shiftRegRightA(uint8_t* reg)
{
	int sevenBit = (*reg >> 7) & 0x1;
	*reg = *reg >> 1;
	flags->C = sevenBit;
	flags->Z = (*reg == 0);
	flags->N = 0;
	flags->H = 0;
	return 8;
}

int Z80InstructionSet::shiftRegRightL(uint8_t* reg)
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

int Z80InstructionSet::testRegBit(uint8_t* reg, int n)
{
	int bit = (*reg >> n) & 0x1;
	flags->Z = (bit == 0);
	flags->N = 0;
	flags->H = 1;
	return 8;
}

int Z80InstructionSet::setRegBit(uint8_t* reg, int n)
{
	int newBit = 1 << n;
	*reg |= newBit;
	return 8;
}

int Z80InstructionSet::resetRegBit(uint8_t* reg, int n)
{
	int newBit = (1 << n) ^ 0xFF;
	*reg &= newBit;
	return 8;
}

/**************************************
 * CPU Control Commands               * 
 **************************************/

int Z80InstructionSet::complementCarry()
{
	flags->C = flags->C ^ 1;
	flags->N = 0;
	flags->H = 0;
	return 4;
}

int Z80InstructionSet::setCarry()
{
	flags->C = 1;
	flags->N = 0;
	flags->H = 0;
	return 4;
}

int Z80InstructionSet::halt()
{
	// TODO implement halt
	return 4;
}

int Z80InstructionSet::stop()
{
	// TODO implement stop
	return -1;
}

int Z80InstructionSet::disableInterrupts()
{
	// intMasEnable = 0;
	return 4;
}

int Z80InstructionSet::enableInterrupts()
{
	// intMasEnable = 1;
	return 4;
}

/**************************************
 * Jump Commands                      * 
 **************************************/

int Z80InstructionSet::jump()
{
	uint8_t jumpLow = memory->memoryRead(registers->PC.val++);
	uint8_t jumpHigh = memory->memoryRead(registers->PC.val++);
	RegisterPair jumpAddr(jumpHigh, jumpLow);
	registers->PC = jumpAddr;
	return 16;
}

int Z80InstructionSet::jumpHL()
{
	registers->PC = registers->HL;
	return 4;
}

int Z80InstructionSet::conditionalJump(int cond)
{
	if(cond)
	{
		return jump();		
	}
	registers->PC.val += 2;
	return 12; 
}

int Z80InstructionSet::relativeJump()
{
	uint8_t jumpOffset = memory->memoryRead(registers->PC.val++);
	registers->PC.val += jumpOffset;
	return 12;
}

int Z80InstructionSet::conditionalRelativeJump(int cond)
{
	if(cond)
	{
		return relativeJump();
	}
	registers->PC.val++;
	return 8;
}

int Z80InstructionSet::call()
{
	registers->PC.val+=2;
	pushToStack(registers->PC.val);
	registers->PC.val-=2;
	jump();
	return 24;
}

int Z80InstructionSet::conditionalCall(int cond)
{
	if(cond)
	{
		return call();
	}
	registers->PC.val += 2;
	return 12;
}

int Z80InstructionSet::returnPC()
{
	popFromStack(&registers->PC);
	return 16;
}

int Z80InstructionSet::conditionalReturnPC(int cond)
{
	if(cond)
	{
		returnPC();
		return 20;
	}
	return 8;
}

int Z80InstructionSet::returnPCI()
{
	enableInterrupts();
	return returnPC();
}

int Z80InstructionSet::sysCall(int address)
{
	pushToStack(registers->PC);
	return 16;
}
