/**
 * \file cpu_z80_inst.h
 * \brief Contains functions to execute the Z80 instruction set.
 *
 * All functions here return the number of CPU cycles the instruction takes.
 */

#ifndef _Z80_INST_
#define _Z80_INST_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "registers.h"

/**
 * The type of a memory address.
 */
typedef uint16_t addr_t;

/**
 * The type of data in reference by an address.
 */
typedef uint8_t data_t;

/**
 * Memory read function.
 * 
 * \param address Memory address to read from.
 */
typedef uint8_t (*mem_read)(uint16_t address);

/**
 * Memory write function.
 *
 * \param address Memory address to write to.
 * \param data Data to write.
 */
typedef void (*mem_write)(uint16_t address, uint8_t data);

/**
 * Initializies the z80 instruction set, must be called before executing any
 * instruction
 *
 * \param z80registers Z80 registers used as part of the CPU.
 * \param z80flags Z80 flags used as part of the CPU.
 * \param read memory read function.
 * \param write memory write function.
 */
void initialize_micro_ops(struct z80Reg *z80registers, struct z80Flags *z80flags,
                          mem_read read, mem_write write);

/**
 * \name 8-bit load
 * All 8-bit load commands.
 */
///@{
/**
 * \brief ld r, r --> r = r
 * 
 * Loads a 8-bit register from another 8-bit register
 *
 * \param src Source register
 * \param dest Destination register.
 */
int loadReg8(uint8_t* src, uint8_t* dest);

/**
 * \brief ld r, n --> r = n
 *
 * Loads the immediate value into a register.
 *
 * \param reg Register to load into.
 */
int loadImmReg8(uint8_t* reg);

/**
 * \brief ld r, (HL) --> r = (HL)
 *
 * Load a register with the value stored in the memory address stored
 * in the register pair HL.
 *
 * \param reg Register to load into.
 */
int loadReg8HL(uint8_t* reg);

/**
 * \brief ld (HL), r --> (HL) = r
 *
 * Store a register value into the memory address stored in the register
 * pair HL.
 * \param reg Register to store.
 */
int storeReg8HL(uint8_t reg);

/**
 * \brief ld (HL), n --> (HL) = n
 *
 * Store, into the memory address stored in the register pair HL, and 
 * an imediate value
 */
int storeHLImm();

/**
 * \brief ld A, (rr) --> A = (rr)
 *
 * Load the A register with the contents of the memory address, stored
 * in the given register pair.
 *
 * \param reg1 The upper 8 bits of the register pair.
 * \param reg2 The lower 8 bits of the register pair.
 */
int loadA(uint8_t reg1, uint8_t reg2); 

/**
 * \brief ld A, (nn) --> A = (nn).
 *
 * Load the imediate value into the A register.
 */
int loadAInd(); 

/**
 * \brief ld (rr), A --> (rr) = A
 *  
 * Store the A register value into the memory address, stored in the given
 * register pair.
 *
 * \param reg1 The upper 8-bits of the register pair.
 * \param reg2 The lower 8-bits of the register pair.
 */
int storeA(uint8_t reg1, uint8_t reg2); 

/**
 * \brief ld (nn), A --> (nn) = A
 *
 * Stores the A register value into the memory address, stored in the 
 * immediate value.
 */
int storeAInd();

/**
 * \brief ld A, (FF00 + n) --> A = (FF00 + n)
 *
 * Stores the value of an IO port into A. The number of the port is determined
 * from the immediate value.
 */
int readIOPortN(); 

/**
 * \brief ld (FF00 + n), A --> (FF00 + n) = A
 *
 * Stores the A register value to an IO port. The number of the port is 
 * determined from the immediate value.
 */
int writeIOPortN();

/**
 * \brief ld A, (FF00 + C) --> (FF00 + flags.C) = A
 *
 * Stores the value of an IO port into A. The number of the port is determined
 * from the carry flag.
 */
int readIOPortC();

/**
 * \brief ld (FF00 + C), A --> (FF00 + flags.C) = A
 *
 * Stores the A register value to an IO port. The number of the port is 
 * determined from the carry flag.
 */
int writeIOPortC();

/**
 * \brief ldi (HL), A --> (HL) = A, HL = HL + 1
 *
 * Stores the A register value into the memory address stored in the HL 
 * register pair, then increments the value of the HL register pair.
 */
int storeIncrement();

/**
 * \brief ldi A, (HL) --> A = (HL), HL = HL + 1
 *
 * Loads the A register with the value in the memory address, stored in the
 * HL register pair, then increments the HL register pair.
 */
int loadIncrement();

/**
 * \brief ldd (HL), A --> (HL) = A, HL = HL - 1
 *
 * Stores the A register value into the memory address stored in the HL 
 * register pair, then decrements the value of the HL register pair.
 */
int storeDecrement();

/**
 * \brief ldd A, (HL) --> A = (HL), HL = HL - 1
 *
 * Loads the A register with the value in the memory address, stored in the
 * HL register pair, then decrements the HL register pair.
 */
int loadDecrement();
///@}

/**
 * @name 16-bit load.
 * All 16-bit load commands.
 */
///@{
/**
 * \brief ld rr, nn --> rr = nn
 * 
 * Load a 16-bit register with the immediate value.
 *
 * \param reg1 The upper bits if the register.
 * \param reg2 The lower bits of the register.
 */
int loadReg16(uint8_t* reg1, uint8_t* reg2);

/**
 * \brief ld SP, nn --> SP = nn
 *
 * Load the immediate value into SP.
 */
int loadSPImm(); 

/**
 * \brief ld SP, HL --> SP = HL
 *
 * Load the value of the register pair HL into SP.
 */
int loadHLToSP();

/**
 * \brief push rr --> SP = SP - 2, (SP) = rr
 *
 * Load a register pair into SP
 *
 * \param reg1 Upper 8 bits of the register pair.
 * \param reg2 Lower 8 bits of the register pair.
 */
int pushToStack(uint8_t reg1, uint8_t reg2);

/**
 * \brief pop rr --> SP = SP + 2, rr = (SP)
 *
 * Pop a 16-bit value from the stack and load the value into a register pair.
 * 
 * \param reg1 Upper 8 bits of the register pair.
 * \param reg2 Lower 8 bits of the register pair
 */
int popFromStack(uint8_t* reg1, uint8_t* reg2);
///@}


/** 
 * \name 8-bit arithmetic/logical
 * All 8-bit arithmetic/logical commands
 */
///@{
/**
 * \brief add A, r --> A = A + r
 *
 * Add a register value to the A register.
 *
 * \param reg The register value to add.
 */
int addReg(uint8_t reg); 

/**
 * \brief add A, n --> A = A + n
 *
 * Add the immediate value to the a regiter.
 */
int addImm(); 

/**
 * \brief add A, (HL) --> A = A + (HL)
 *
 * Add the contents of the memory address, stored in the register pair HL, 
 * to the A register.
 */
int addHLInd(); 

/**
 * \brief adc A, r --> A = A + r + cy
 *
 * Add a register value as well as the carry flag to the A register.
 *
 * \param reg The register value to add.
 */
int adcReg(uint8_t reg);

/**
 * \brief adc A, n --> A = A + n + cy
 *
 * Add the immediate value as well as the carry flag to the A register.
 */
int adcImm(); 

/**
 * \brief adc A, HL --> A = A + (HL) + cy
 *
 * Add the contents of the memory address, stored in the register pair HL, 
 * as well as the carry flag to the A register.
 */
int adcHLInd(); 

/**
 * \brief sub r --> A = A - r
 *
 * Subtract a register value from the A register.
 *
 * \param reg Register value to subtract.
 */
int subReg(uint8_t reg); 

/**
 * \brief sub n --> A = A - n
 *
 * Subtract the immediate value from the A register.
 */
int subImm(); 

/**
 * \brief sub (HL) --> A = A - (HL)
 *
 * Subtract the contents of memory address stored in the register pair HL, 
 * from the A register.
 */
int subHLInd(); 

/**
 * \brief sbc A, r --> A = A - r - cy
 *
 * Subtract a register value and the carry flag from the A register.
 */
int sbcReg(uint8_t reg);

/**
 * \brief sbc A, n --> A = A - n - cy
 *
 * Subtract the immediate value and the carry value from the A register.
 */
int sbcImm();

/**
 * \brief sbc A, (HL) --> A = A - (HL) - cy
 *
 * Subtract the contents of the memory address, stored in the register pair HL, 
 * and the carry flag from the A register.
 */
int sbcHLInd();

/**
 * \brief and r --> A = A & r
 *
 * Perform a bitwise and between a register value and the A register.
 */
int andReg(uint8_t reg);

/**
 * \brief and n --> A = A & n
 *
 * Perform a bitwise and between the immediat value and the A register.
 */
int andImm();

/**
 * \brief and (HL) --> A = A & (HL)
 *
 * Perform a bitwise and between the contents of the memory address, stored in 
 * the register pair HL, and the A register.
 */
int andHLInd();

/**
 * \brief xor r --> A = A ^ r
 *
 * Perform a bitwise xor beteen a register value and the A register.
 */
int xorReg(uint8_t reg);

/**
 * \brief xor n --> A = A ^ n
 *
 * Perform a bitwise xor between the immediate value and the A register.
 */
int xorImm();

/**
 * \brief xor (HL) --> A = A ^ (HL)
 *
 * Perform a bitwise xor between the contents of the memory address, stored in
 * the register pair HL, and the A register.
 */
int xorHLInd();

/**
 * \brief or r --> A = A | r
 *
 * Perform a bitwise or beteen a register value and the A register. 
 */
int orReg(uint8_t reg);

/**
 * \brief or n --> A = A | n
 *
 * Perform a bitwise or between the immediate value and the A register.
 */
int orImm();

/**
 * \brief or (HL) --> A = A | (HL)
 *
 * Perform a bitwise or between the contents of the memory address, stored in
 * the register pair HL, and the A register.
 */
int orHLInd();

/**
 * \brief cp r --> compare A - r
 *
 * Compare a register value to the A register. This will set flags as if a 
 * subtraction has occurred, but will not modify the value of the A register.
 */
int compareReg(uint8_t reg);

/**
 * \brief cp n --> compare A - n
 *
 * Compare the immediate to the A register. This will set flags as if a 
 * subtraction has occurred, but will not modify the value of the A register.
 */
int compareImm();

/**
 * \brief cp (HL) --> compare A - (HL)
 *
 * Compare the contentes of the memory address, stored in the register pair HL,
 * to the A register. This will set flags as if a subtraction has occurred, but
 * will not modify the value of the A register.
 */
int compareHLInd();

/**
 * \brief inc r --> r = r + 1
 *
 * Increment a register value by 1.
 *
 * \param reg Register to increment.
 */
int incReg8(uint8_t* reg);

/**
 * \brief inc (HL) --> (HL) = (HL) + 1
 *
 * Increment the contents of the memory address stored in the register pair HL
 * by 1.
 */
int incHLInd();

/**
 * \brief dec r --> r = r - 1
 *
 * Decrement a register value by 1.
 *
 * \param reg Register to decrement.
 */
int decReg8(uint8_t* reg);

/**
 * \brief dec (HL) --> (HL) = (HL) - 1
 *
 * Decrement the contents of the memory address stored in the register pair HL
 * by 1.
 */
int decHLInd();

/**
 * \brief daa
 *
 * Decimally Adjust the register A. The adjustment is determined by the CPU flags.
 */
int decimalyAdjustA();

/**
 * \brief cpl --> A = A ^ 0xFF
 *
 * Complement the register A, perform an xor between the value 0xFF and the 
 * A register.
 */
int complementA();
///@}

/**
 * \name 16-bit arithmetic/logical
 * All 16-bit arithmetic/logical commands
 */
///@{
/**
 * \brief add HL, rr --> HL = HL + rr
 *
 * Add a register pair to the registerh pair HL.
 *
 * \param reg1 The upper eight bits of the register pair.
 * \param reg2 The lower eight bits of the register pair.
 */
int addReg16(uint8_t reg1, uint8_t reg2);

/**
 * \brief inc rr --> rr = rr + 1
 *
 * Increment a register pair by 1.
 *
 * \param reg1 The upper eight bits of a register pair.
 * \param reg2 The lower eight bits of a register pair.
 */
int incReg16(uint8_t* reg1, uint8_t* reg2);

/**
 * \brief dec rr --> rr = rr - 1
 *
 * Decrement a register pair by 1.
 * 
 * \param reg1 Upper eight bits of the register pair.
 * \param reg2 Lower eight bits of the register pair.
 */
int decReg16(uint8_t* reg1, uint8_t* reg2);

/**
 * \brief add SP, dd --> SP =  SP +/- dd
 *
 * Add the immediate value to SP.
 */
int addToSP();

/**
 * \brief ld HL, SP+dd --> HL = SP +/- dd
 *
 * Add the immediate value to SP, then load SP into the register pair HL.
 */
int addSPToHL();
///@}

/** 
 * \name Rotate and shift
 * All Rotate and shift commands.
 */
///@{
/**
 * \brief rlca
 *
 * Rotates the A register to the left one place. The 7th bit is put back into 
 * the 0 position. The 7th bit also goes to the carry flag.
 */
int rotateALeftC();

/**
 * \brief rla
 *
 * The bits in the A register are all rotated left, the 7th bit goes
 * to the carry flag and the carry flag goes to bit 0. 
 */
int rotateALeft();

/**
 * \brief rrca
 *
 * Rotates the A register to the right one place. The 0 bit is put into the 
 * 7th position. The 0 bit also goes to the carry flag.
 */
int rotateARightC();

/**
 * \brief rra
 *
 * The A register is shifted right by one, the 0 bit goes to the carry
 * flag, and the carry flag goes to bit 7.
 */
int rotateARight();

/**
 * \brief rlc r
 *
 * Rotates a register to the left one place. The 7th bit is put back into 
 * the 0 position. The 7th bit also goes to the carry flag.
 *
 * \param reg Register to rotate.
 */
int rotateRegLeftC(uint8_t* reg);

/**
 * \brief rl r
 *
 * The bits in a register are all rotated left, the 7th bit goes
 * to the carry flag and the carry flag goes to bit 0.
 *
 * \param reg Register to rotate.
 */
int rotateRegLeft(uint8_t* reg);

/**
 * \brief rrc r
 *
 * Rotates a register to the right one place. The 0 bit is put into the 
 * 7th position. The 0 bit also goes to the carry flag.
 *
 * \param reg Register to rotate.
 */
int rotateRegRightC(uint8_t* reg);

/**
 * \brief rr r
 *
 * A register is shifted right by one, the 0 bit goes to the carry
 * flag, and the carry flag goes to bit 7.
 *
 * \param reg Register to shift.
 */
int rotateRegRight(uint8_t* reg);

/**
 * \brief sla r --> shift left arithmetic
 *
 * Shifts a register left on place. The 7th bit goes into the carry flag.
 * The 0 bit has 0 shifted in.
 *
 * \param reg Register to shift.
 */
int shiftRegLeftA(uint8_t* reg);

/**
 * \brief swap r --> exchange low/high-nibble
 * 
 *
 */
int swapReg(uint8_t* reg);

/**
 * \brief sra r --> shift right arithmetic
 *
 * Shifts a register right on place. The 0 bit goes into the carry flag.
 * The 7th bit stays the same.
 * 
 * \param reg Register to shift.
 */
int shiftRegRightA(uint8_t* reg);

/**
 * \brief srl r --> shift right logical
 *
 * Shifts a register right on place. The 0 bit goes into the carry flag.
 * The 7th bit goes to 0.
 * 
 * \param reg Register to shift.  
 */
int shiftRegRightL(uint8_t* reg);
///@}

/**
 * \name Single bit
 * All single bit operations.
 */
///@{
/**
 * \brief bit n, r --> test bit n of r
 *
 * Tests if bit n of a register is set.
 *
 * \param reg Register to test.
 * \param n Bit position.
 */
int testRegBit(uint8_t* reg, int n);

/**
 * \brief set n, r --> set bit n of (HL)
 *
 * Sets bit n of a register.
 * 
 * \param reg Register to set.
 * \param n Bit position.
 */
int setRegBit(uint8_t* reg, int n);

/**
 * \brief res n, r --> reset bit n of r
 *
 * Clears bit n of a register.
 *
 * \param reg Register to clear
 * \param n Bit position.
 */
int resetRegBit(uint8_t* reg, int n);
///@}

/**
 * \name Cpu control
 * All cpu control commands.
 */
///@{
/**
 * \brief ccf --> cy = cy ^ 1
 *
 *
 */
int complementCarry();

/**
 * \brief scf --> cy = 1
 *
 *
 */
int setCarry();

/**
 * \brief halt
 *
 * Halt until an interrupt occurs.
 */
int halt();

/**
 * \brief stop
 *
 * Low power standby mode.
 */
int stop();

/**
 * \brief di
 *
 * Disable all interrupts.
 */
int disableInterrupts();

/**
 * \brief ei
 *
 * Enable all interrupts.
 */
int enableInterrupts();
///@}

/**
 * \name Jump
 * All jump commands.
 */
///@{
/**
 * \brief jp nn --> jump to nn, PC = nn
 *
 * Jump to the immediate value. 
 */
int jump();

/**
 * \brief jp HL --> jump to HL, PC = HL
 *
 * Jump to the value stored in the register pair HL.
 */
int jumpHL();

/**
 * \brief jp f, nn
 *
 * Jump to the immediate value if the condition is true.
 *
 * \param cond 1 if true, 0 if false. Should be a CPU flag.
 */
int conditionalJump(int cond);

/**
 * \brief jr PC+dd --> relative jump to nn (PC=PC+/-7bit)
 *
 * Jump by the immediate value.
 */
int relativeJump();

/**
 * \brief jr f, PC+dd
 *
 * Jump by the immediate value if the condition is true.
 *
 * \param cond 1 if true, 0 if false. Should be a CPU flag.
 */
int conditionalRelativeJump(int cond);

/**
 * \brief call nn --> SP = SP - 2, (SP) = PC, PC = nn
 *
 * Jump to the immediate value after pushing the current PC to the stack.
 */
int call();

/**
 * \brief call f, nn
 *
 * Jump to the immediate value after pushing the current PC to the stack, only 
 * if the condition is true.
 *
 * \param cond 1 if true, 0 if false. Should be a CPU flag.
 */
int conditionalCall(int cond);

/**
 * \brief ret --> PC = (SP), SP = SP + 2
 *
 * Return to an address stored on the stack.
 */
int returnPC();

/**
 * \brief ret f
 *
 * Return to an address stored on the stack, only if the condition is true.
 *
 * \param cond 1 if true, 0 if false. Should be a CPU flag.
 */
int conditionalReturnPC(int cond);

/**
 * \brief reti
 *
 * Return and enable interrupts
 */
int returnPCI();

/**
 * \brief rst --> call to 00, 08, 10, 18, 20, 28, 30, 38
 *
 * System call to preset locations.
 */
int sysCall(int address);
///@}

#endif
