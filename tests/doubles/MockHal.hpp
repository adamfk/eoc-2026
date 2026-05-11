#pragma once
#include "Hal.hpp"
#include <gmock/gmock.h>

class MockHal : public Hal
{
public: // don't forget this!
    MOCK_METHOD(void, SetOutput, (uint8_t index, bool state), (override));
};
