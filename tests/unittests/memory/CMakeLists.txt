# Where the source code for Memory is
set(MEM_DIR ../../../src/Memory)

# Way too many memory files, I'll just use GLOB instead of listing
# them out. Feel free to change Michael
file(GLOB_RECURSE MEMORY_SRCS ${MEM_DIR}/*.h ${MEM_DIR}/*.cpp)

set(MEMORY_TEST_SRCS allMbcMemoryTest.cc
                     mbc1MemoryTest.cc
                     mbc2MemoryTest.cc
                     memoryTestBase.h
                     noMbcMemoryTest.cc
   )

# Build Memory Tests
add_executable(memoryTests ${MEMORY_SRCS} ${MEMORY_TEST_SRCS})
target_link_libraries(memoryTests gtest_main)

# Add test so they can be run with ctest
add_test(memoryTests ${CMAKE_CURRENT_DIRECTORY}/memoryTests)
