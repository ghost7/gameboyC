#ifndef _MEMORY_DEFS_H_
#define _MEMORY_DEFS_H_

#include <stdint.h>

const int KB_SIZE = 0x400;
typedef uint8_t data_t;
typedef uint16_t addr_t;

enum BankingMode {
    ROM_BANKING_MODE = 0x00,
    RAM_BANKING_MODE = 0x01
};

#endif
