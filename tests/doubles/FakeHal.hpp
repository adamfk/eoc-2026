#pragma once

#include "Hal.hpp"

#include <array>
#include <string>
#include <gtest/gtest.h>

class FakeHal : public Hal
{
public:
    std::array<bool, Hal_MAX_OUTPUTS> outputs{}; // The {} zero-initializes all elements

    void SetOutput(uint8_t index, bool state) override
    {
        outputs.at(index) = state; // will throw an exception on bad index. Will fail test, but allows other tests to continue.
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
