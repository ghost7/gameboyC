#ifndef __FLAG_OP_TEST_BASE__
#define __FLAG_OP_TEST_BASE__

#include "microOpTestBase.h"

/**
 * Base Test class for all micro operations that affect flags.
 */
class FlagOpTestBase : public ::MicroOpTestBase
{
protected:
    /**
     * Test set up.
     */
    virtual void SetUp();

    /**
     * Sets the test carry flag.
     */
    void SetCTestFlag();
    
    /**
     * Sets the test zero flag.
     */
    void SetZTestFlag();
    
    /**
     * Sets the test negation flag.
     */
    void SetNTestFlag();
    
    /**
     * Sets the test half-carry flag.
     */
    void SetHTestFlag();
    
    /**
     * Clears the test carry flag.
     */
    void ClearCTestFlag();

    /**
     * Clears the test zero flag.
     */
    void ClearZTestFlag();
    
    /**
     * Clears the test negative flag.
     */   
    void ClearNTestFlag();
    
    /**
     * Clears the test half-carry flag.
     */   
    void ClearHTestFlag();
    
    /**
     * Clears the test flags.
     */   
    void ClearAllFlags();
    
    /**
     * Checks if the test flags and the real flags are equal.
     */
    void CheckFlags();

    /**
     * Test flags.
     */
    struct Z80Flags *testFlags;
};

#endif
