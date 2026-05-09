#pragma once

extern "C" // this lets C++ link with C
{
#include "Hal.h"
}

#include <array>
#include <string>
#include <stdexcept>
#include <gtest/gtest.h>

// My style choice when mixing C and C++ for C devs learning to use C++:
//      `snake_case` for for C 
//      `camelCase`/`PascalCase` for C++
// Use .hpp for C++. https://www.youtube.com/watch?v=mr3sOT-Delg

// If you had a lot of code here or a large project that took a long 
// time to compile, would be good to move this code into .cpp file.

class FakeHal
{
private:
    // Global pointer for connecting C calls to a FakeHal object. Replaceable singleton.
    static FakeHal *g_singleton;

public:
    std::array<bool, Hal_MAX_OUTPUTS> outputs{}; // The `{}` zero-initializes all elements.

    // constructor setups up global pointer for C integration
    FakeHal()
    {
        if (g_singleton != nullptr)
        {
            throw std::runtime_error("You can only have a single FakeHal instance at a time due to simple C API");
        }
        g_singleton = this;
    }

    // destructor nulls global pointer
    ~FakeHal()
    {
        g_singleton = nullptr;
    }

    void SetOutput(uint8_t index, bool state)
    {
        // non-throwing options:
        // ASSERT_LT(index, outputs.size());  // alternative to throwing. Provides detail, but allows current test to continue.
        // outputs[index] = state; // will give undefined behavior on bad index.

        // throwing
        outputs.at(index) = state; // will throw an exception on bad index. Will fail current test, but allows other tests to continue.
    }

    // throws an exception if user forgets construct a FakeHal object first
    static FakeHal &GetGlobalInstance()
    {
        if (g_singleton == nullptr)
        {
            throw std::runtime_error("no FakeHal singleton created yet");
        }
        return *g_singleton;
    }

    const std::string GetOutputsString()
    {
        std::string str;
        str.reserve(outputs.size()); // not actually needed in this case because of Small String Optimization

        for (bool output : outputs)
        {
            str += output ? "#" : "_";
        }

        return str;
    }

    void AssertOutputs(std::string_view string)
    {
        ASSERT_EQ(string, GetOutputsString());
    }
};
