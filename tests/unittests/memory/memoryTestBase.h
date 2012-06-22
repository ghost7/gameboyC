#ifndef __MEMORY_TEST_BASE_H_
#define __MEMORY_TEST_BASE_H_

#include "../../include/gtest/gtest.h"
#include "../../../src/Memory/memoryHeaders.h"

// call these macros from inside a TEST
#define ASSERT_RO( addr ) data_t start = mem->read( addr ); \
                        mem->write( addr, (data_t)(start+10) ); \
                        ASSERT_EQ( start, mem->read( addr ) ) << "Address " << addr << " should be read only, but I wrote to it.";

// TODO: check the overflow behavior
#define ASSERT_RW( addr ) data_t start = mem->read( addr ); \
                        mem->write( addr, (data_t)(start+10) ); \
                        ASSERT_EQ( start+10, mem->read( addr ) ) << "Address " << addr << " should be read/write, but I couldn't write to it.";


class MemoryTestBase : public ::testing::Test {

protected:

    /** Use thie to read and write memory across the entire 
        address space per the intricacies of the GB memory map */
    MemoryInterface* mem;

    virtual void SetUp() { 
        // TODO: setup? new
        // TODO: figure out how to specify which tests for the different controllers
        //mem = loadCartridge( "ROM_ONLY+0x0020KB_ROM+0x0000KB_RAM.gb" );
        mem = loadCartridge( "MBC1+0x0100KB_ROM+0x0008KB_RAM.gb" );
    }

};

#endif
