#ifndef _MEMORY_HEADERS_H_
#define _MEMORY_HEADERS_H_

#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


const int KB_SIZE = 0x400;
typedef uint8_t data_t;
typedef uint16_t addr_t;

enum BankingMode {
    ROM_BANKING_MODE = 0x00,
    RAM_BANKING_MODE = 0x01
};


#include "cartridge.h"
#include "memoryInterface.h"
#include "memory.h"
#include "mbc1Memory.h"
#include "mbc2Memory.h"
#include "mbc3Memory.h"
#include "mbc5Memory.h"


#endif
