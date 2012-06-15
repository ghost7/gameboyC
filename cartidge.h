/**
 * \file cartidge.h
 * \brief Contains functions to load and manipulate a gameboy cartidge.
 */

#ifndef _GB_CARTIDGE_
#define _GB_CARTIDGE_

#include <stdint.h>

/** 
 * Loads a gameboy cartidge from a rom file.
 *
 * \param filename The name of the rom file.
 */
struct gb_cartidge *load_cartidge(const char *filename);

/**
 * Reads memory from the cartidge.
 *
 * \param cartidge The cartidge to read from.
 * \param memAddr The memory address to read from.
 */
uint8_t cartidge_read(struct gb_cartidge* cartidge, int memAddr);

/**
 * Writes to a cartidge.
 *
 * \param cartidge The cartidge to write to.
 * \param memAddr the memory address to write to.
 * \param write_data the data to write.
 */
void cartidge_write(struct gb_cartidge *cartidge, int memAddr, uint8_t write_data);

/**
 * Prints general information about a cartidge, such as its title and rom size.
 *
 * \param cartidge The cartidge to print.
 */
void cartidge_print(struct gb_cartidge *cartidge);

#endif
