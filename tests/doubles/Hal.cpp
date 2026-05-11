#include "Hal.hpp"

// Allocate global pointer for FakeHal.
Hal* Hal::g_singleton = nullptr;

// NOTE!
// Below functions have C linkage, but may throw exceptions due to FakeHal C++.
// If you have problems with this, try compiling C code with `-fexceptions` (gcc).
// or you can modify code to use gtest ASSERT_X macros which mark a failure and return.

void Hal_set_output(uint8_t index, bool state)
{
    Hal::GetGlobalInstance().SetOutput(index, state);
}
