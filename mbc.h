/**
 * \file mbc.h
 * \brief Contains declarations for all cartidge read and writes.
 *
 * A pointer to one of these reads/writes is set as the read and 
 * write member of the gb_cartidge struct.
 */


#ifndef _MBC_H_
#define _MBC_H_

#include <stdint.h>
#include "cartidge.h"

/**
 * Cartidge read function.
 *
 * \param gameData Data to read from.
 * \param memAddr Memory address to read from.
 */
typedef uint8_t (*read_data)(uint8_t *gameData, int memAddr);

/**
 * Cartidge write function.
 *
 * \param gameData Data to write to.
 * \param memAddr Memory address to write to.
 * \param data Data to write.
 */
typedef void (*write_data)(uint8_t *gameData, int memAddr, uint8_t data);

/**
 * Read from a ROM cartidge.
 *
 * \param gameData Data stored in the cartidge.
 * \param memAddr Memory address to read from
 */
uint8_t ROM_memoryRead(uint8_t* gameData, int memAddr);

/**
 * Read from a MBC1 cartidge.
 *
 * \param gameData Data stored in the cartidge.
 * \param memAddr Memory address to read from
 */
uint8_t MBC1_memoryRead(uint8_t* gameData, int memAddr);

/**
 * Read from a MBC2 cartidge.
 *
 * \param gameData Data stored in the cartidge.
 * \param memAddr Memory address to read from
 */
uint8_t MBC2_memoryRead(uint8_t* gameData, int memAddr);

/**
 * Read from a MBC3 cartidge.
 *
 * \param gameData Data stored in the cartidge.
 * \param memAddr Memory address to read from
 */
uint8_t MBC3_memoryRead(uint8_t* gameData, int memAddr);

/**
 * Read from a MBC5 cartidge.
 *
 * \param gameData Data stored in the cartidge.
 * \param memAddr Memory address to read from
 */
uint8_t MBC5_memoryRead(uint8_t* gameData, int memAddr);

/**
 * Write to a ROM cartidge.
 *
 * \param gameData Data stored in the cartidge.
 * \param memAddr Memory address to read from.
 * \param writeData Data to write.
 */
void ROM_memoryWrite(uint8_t* gameData, int memAddr, uint8_t writeData);

/**
 * Write to a MBC1 cartidge.
 *
 * \param gameData Data stored in the cartidge.
 * \param memAddr Memory address to read from.
 * \param writeData Data to write.
 */
void MBC1_memoryWrite(uint8_t* gameData, int memAddr, uint8_t writeData);

/**
 * Write to a MBC2 cartidge.
 *
 * \param gameData Data stored in the cartidge.
 * \param memAddr Memory address to read from.
 * \param writeData Data to write.
 */
void MBC2_memoryWrite(uint8_t* gameData, int memAddr, uint8_t writeData);

/**
 * Write to a MBC3 cartidge.
 *
 * \param gameData Data stored in the cartidge.
 * \param memAddr Memory address to read from.
 * \param writeData Data to write.
 */
void MBC3_memoryWrite(uint8_t* gameData, int memAddr, uint8_t writeData);

/**
 * Write to a MBC5 cartidge.
 *
 * \param gameData Data stored in the cartidge.
 * \param memAddr Memory address to read from.
 * \param writeData Data to write.
 */
void MBC5_memoryWrite(uint8_t* gameData, int memAddr, uint8_t writeData);

#endif
