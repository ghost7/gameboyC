#include "flagOpTestBase.h"

void FlagOpTestBase::SetUp()
{
    MicroOpTestBase::SetUp();
    ClearAllFlags();
}

void FlagOpTestBase::SetCTestFlag()
{
    testFlags.C = 1;
}

void FlagOpTestBase::SetZTestFlag()
{
    testFlags.Z = 1;
}

void FlagOpTestBase::SetNTestFlag()
{
    testFlags.N = 1;
}

void FlagOpTestBase::SetHTestFlag()
{
    testFlags.H = 1;
}

void FlagOpTestBase::SetAllFlags(struct z80Flags newTestFlags)
{
    testFlags = newTestFlags;
}

void FlagOpTestBase::ClearCTestFlag()
{
    testFlags.C = 0;
}

void FlagOpTestBase::ClearZTestFlag()
{
    testFlags.Z = 0;
}

void FlagOpTestBase::ClearNTestFlag()
{
    testFlags.N = 0;
}

void FlagOpTestBase::ClearHTestFlag()
{
    testFlags.H = 0;
}

void FlagOpTestBase::ClearAllFlags()
{
    ClearCTestFlag();
    ClearZTestFlag();
    ClearNTestFlag();
    ClearHTestFlag();
}

void FlagOpTestBase::CheckFlags()
{
    ASSERT_EQ(flags.C, testFlags.C);
    ASSERT_EQ(flags.Z, testFlags.Z);
    ASSERT_EQ(flags.N, testFlags.N);
    ASSERT_EQ(flags.H, testFlags.H);
}
