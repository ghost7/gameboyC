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
     * Sets all the test flags to the specified value.
     * \param newTestFlags new value of the test flags.
     */
    void SetAllFlags(struct z80Flags newTestFlags);
    
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

private:
    /**
     * Test flags.
     */
    struct z80Flags testFlags;
};

#endif
