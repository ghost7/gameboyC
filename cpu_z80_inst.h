/**
 * Z80-Instruction Set
 */

#ifndef _Z80_INST_
#define _Z80_INST_

#include <stdint.h>

// 8-bit load commands
int loadReg8(int8_t* src, int8_t* dest);	// ld r, r		r = r
int loadImmReg8(int8_t* reg);				// ld r, n		r = n
int loadReg8HL(int8_t* reg);				// ld r, (HL)	r = (HL)
int storeReg8HL(int8_t reg);				// ld (HL), r	(HL) = r
int storeHLImm();							// ld (HL), n	(HL) = n
int loadA(int8_t reg1, int8_t reg2);		// ld A, (rr)	A = (rr)
int loadAInd();								// ld A, (nn)	A = (nn)
int storeA(int8_t reg1, int8_t reg2);		// ld (rr), A	(rr) = A
int storeAInd();							// ld (nn), A	(nn) = A
int readIOPortN();							// ld A, (FF00 + n)
int writeIOPortN();							// ld (FF00 + n), A
int readIOPortC();							// ld A, (FF00 + C)
int writeIOPortC();							// ld (FF00 + C), A
int storeIncrement();						// ldi (HL), A	(HL) = A, HL = HL + 1
int loadIncrement();						// ldi A, (HL)	A = (HL), HL = HL + 1
int storeDecrement();						// ldd (HL), A	(HL) = A, HL = HL - 1
int loadDecrement();						// ldd A, (HL)	A = (HL), HL = HL - 1

// 16-bit load commands
int loadReg16(int8_t* reg1, int8_t* reg2);	// ld rr, nn
int loadSPImm();							// ld SP, nn
int loadHLToSP();							// ld SP, HL
int pushToStack(int8_t reg1, int8_t reg2);	// push 		SP = SP - 2, (SP) = rr			
int popFromStack(int8_t* reg1, int8_t* reg2);// pop			SP = SP + 2, rr = (SP)

// 8-bit arithmetic/logical commands
int addReg(int8_t reg);						// add A, r		A = A + r
int addImm();								// add A, n		A = A + n
int addHLInd();								// add A, (HL)	A = A + (HL)
int adcReg(int8_t reg);						// adc A, r		A = A + r + cy
int adcImm();								// adc A, n		A = A + n + cy
int adcHLInd();								// adc A, HL	A = A + (HL) + cy
int subReg(int8_t reg);						// sub r		A = A - r
int subImm();								// sub n		A = A - n
int subHLInd();								// sub (HL)		A = A - (HL)
int sbcReg(int8_t reg);						// sbc A, r		A = A - r - cy
int sbcImm();								// sbc A, n		A = A - n - cy
int sbcHLIndA();							// sbc A, (HL)	A = A - (HL) - cy
int andReg(int8_t reg);						// and r		A = A & r
int andImm();								// and n		A = A & n
int andHLInd();								// and (HL)		A = A & (HL)
int xorReg(int8_t reg);						// xor r		A = A ^ r
int xorImm();								// xor n		A = A ^ n
int xorHLInd();								// xor (HL)		A = A ^ (HL)
int orReg(int8_t reg);						// or r			A = A | r
int orImm();								// or n			A = A | n
int orHLInd();								// or (HL)		A = A | (HL)
int compareReg(int8_t reg);					// cp r			compare A - r
int compareImm();							// cp n			compare A - n
int compareHLInd();							// cp (HL)		compare A - (HL)
int incReg8(int8_t* reg);					// inc r		r = r + 1
int incHLInd();								// inc (HL)		(HL) = (HL) + 1
int decReg8(int8_t* reg);					// dec r		r = r - 1
int decHLInd();								// dec (HL)		(HL) = (HL) - 1
int decimalyAdjustA();						// daa
int complementA();							// cpl			A = A ^ 0xFF

// 16-bit arithmetic/logical commands
int addReg16(int8_t reg1, int8_t reg2);		// add HL, rr	HL = HL + rr
int incReg16(int8_t* reg1, int8_t* reg2);	// inc rr		rr = rr + 1
int decReg16(int8_t* reg1, int8_t* reg2);	// dec rr		rr = rr - 1	
int addToSP();								// add SP, dd	SP =  SP +/- dd
int addSPToHL();							// ld HL, SP+dd	HL = SP +/- dd

// rotate and shift commands
int rotateALeftC();							// rlca
int rotateALeft();							// rla
int rotateARightC();						// rrca
int rotateARight();							// rra
int rotateRegLeftC(int8_t* reg);			// rlc r
int rotateRegLeft(int8_t* reg);				// rl r
int rotateRegRightC(int8_t* reg);			// rrc r
int rotateRegRight(int8_t* reg);			// rr r
int shiftRegLeftA(int8_t* reg);				// sla r		shift left arithmetic
int swapReg(int8_t* reg);					// swap r		exchange low/high-nibble
int shiftRegRightA(int8_t* reg);			// sra r		shift right arithmetic
int shiftRegRightL(int8_t* reg);			// srl r		shift right logical

// single bit operations
int testRegBit(int8_t* reg, int n);			// bit n, r		test bit n of r
int testHLIndBit(int n);					// bit n, (HL)	test bit n of (HL)
int setRegBit(int8_t* reg, int n);			// set n, r		set bit n of (HL)
int setHLIndBit(int n);						// set n (HL)	set bit n of r
int resetRegBit(int8_t* reg, int n);		// res n, r		reset bit n of r
int resetHLIndBit(int n);					// res n, (HL)	reset bit n of (HL)

// cpu control commands
int complementCarry();						// ccf			cy = cy ^ 1
int setCarry();								// scf			cy = 1
int halt();									// halt			halt until an interrupt occurs
int stop();									// stop			low power standby mode
int disableInterrupts();					// di
int enableInterrupts();						// ei

// jump commands
int jump();									// jp nn		jump to nn, PC = nn
int jumpHL();								// jp HL		jump to HL, PC = HL
int conditionalJump(int cond);				// jp f, nn
int relativeJump();							// jr PC+dd		relative jump to nn (PC=PC+/-7bit)
int conditionalRelativeJump(int cond);		// jr f, PC+dd
int call();									// call nn		call to nn, SP = SP - 2, (SP) = PC, PC = nn
int conditionalCall(int cond);				// call f, nn
int returnPC();								// ret			return, PC = (SP), SP = SP + 2
int conditionalReturnPC(int cond);			// ret f
int returnPCI();							// reti			return and enable interrupts
int sysCall(int address);					// rst			call to 00, 08, 10, 18, 20, 28, 30, 38

#endif
