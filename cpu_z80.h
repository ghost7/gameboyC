/**
 * \file cpu_z80.h
 * \brief Contains functions to run the Z80 cpu.
 */

#ifndef CPU_Z80_H
#define CPU_Z80_H

/**
 * Initializes the Z80 CPU.
 */
void initCpu();

/**
 * Checks for any interrupts.
 * 
 * \return number of cycles needed to service any interrupts.
 * Will be 0 if no interrupt has occured.
 */
int checkInterrupts();

/**
 * Fetches the next instruction and executes it.
 *
 * \return The number of cycles needed to execute the instruction.
 */
int cpuStep();

#endif
