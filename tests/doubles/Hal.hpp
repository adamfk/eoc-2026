#pragma once

extern "C"
{
#include "Hal.h"
}

#include <stdint.h>
#include <stdexcept>

// interface for HAL.
// Even if you aren't planning to add mocks, this "interface" (abstract class) is kinda nice to have separate from FakeHal.
// It allows FakeHal to focus on business logic and not lifetimes or c linkage.
class Hal 
{
public:
    // constructor setups up global pointer for C integration
    Hal()
    {
        if (g_singleton != nullptr)
        {
            throw std::runtime_error("You can only have a single FakeHal instance at a time due to simple C API");
        }
        g_singleton = this;
    }

    // destructor nulls global pointer
    ~Hal()
    {
        g_singleton = nullptr;
    }    

    // "pure virtual" method. Class that inherit from this must class must override.
    virtual void SetOutput(uint8_t index, bool state) = 0;

    // throws an exception if user forgets construct a FakeHal object first
    static Hal &GetGlobalInstance()
    {
        if (g_singleton == nullptr)
        {
            // could also change function to return null instead
            throw std::runtime_error("no g_fake_hal created yet");
        }
        return *g_singleton;
    }

private:
    // global pointer for connecting C calls to a FakeHal object
    static Hal *g_singleton;
};
