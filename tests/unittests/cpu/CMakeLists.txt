# Where the source code for the CPU is
set(CPU_DIR ../../../src/Cpu)

# Source code for MicroOp Tests.
set(MICRO_OP_SRCS ${CPU_DIR}/Z80InstructionSet.cpp)
set(MICRO_OP_TEST_DIR microOps)
set(MICRO_OP_TESTS_SRCS ${MICRO_OP_TEST_DIR}/eightBitLoadTests.cc
                        ${MICRO_OP_TEST_DIR}/eightBitMathTests.cc
                        ${MICRO_OP_TEST_DIR}/flagOpTestBase.cc
                        ${MICRO_OP_TEST_DIR}/flagOpTestBase.h
                        ${MICRO_OP_TEST_DIR}/mathFlagsTests.cc
                        ${MICRO_OP_TEST_DIR}/microOpTestBase.cc
                        ${MICRO_OP_TEST_DIR}/microOpTestBase.h
                        ${MICRO_OP_TEST_DIR}/rotateAndShiftTests.cc
                        ${MICRO_OP_TEST_DIR}/sixteenBitLoadTests.cc
                        ${MICRO_OP_TEST_DIR}/sixteenBitMathTests.cc
   )

# Source code for Register tests
set(REGISTER_SRCS ${CPU_DIR}/Z80.h)
set(REGISTER_TEST_DIR register)
set(REGISTER_TEST_SRCS ${REGISTER_TEST_DIR}/registerTests.cc)

# Build MicroOpTests
add_executable(microOpTests ${MICRO_OP_SRCS} ${MICRO_OP_TESTS_SRCS})
target_link_libraries(microOpTests gtest_main)

# Build Register Tests 
add_executable(registerTests ${REGISTER_TEST_SRCS})
target_link_libraries(registerTests gtest_main)

# Add tests so they can be run with ctest, 
add_test(microOpTests ${CMAKE_CURRENT_DIRECTORY}/microOpTests)
add_test(registerTests ${CMAKE_CURRENT_DIRECTORY}/registerTests)
