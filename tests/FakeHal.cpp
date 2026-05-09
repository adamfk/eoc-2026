extern "C" // this lets C++ link with C
{
#include "Hal.h"
}

#include "FakeHal.hpp"

// Allocate global pointer for FakeHal. Replaceable singleton.
// This needs to be done in .cpp file (can't be in .hpp file).
// Actually, it could be done in C++17 .hpp file with `static inline`, but I want to keep things "simple".
FakeHal* FakeHal::g_singleton = nullptr;

// NOTE!
// Below functions have C linkage, but may throw exceptions due to FakeHal C++.
// If you have problems with this, try compiling C code with `-fexceptions` (gcc).
// or you can modify code to use gtest ASSERT_X macros which mark a failure and return.

void Hal_set_output(uint8_t index, bool state)
{
    FakeHal::GetGlobalInstance().SetOutput(index, state);
}
