extern "C"
{
#include "Lights.h"
}
#include "doubles/FakeHal.hpp"
#include <gtest/gtest.h>

// we create a test fixture so that FakeHal is constructed fresh for every test.
// we also put a small helper in there.
class LightTests : public ::testing::Test
{
protected:
    FakeHal hal;

    void AssertLights(std::string_view lights)
    {
        hal.AssertOutputs(lights);
    }
};

TEST_F(LightTests, TestSetup)
{
    // ensure HAL starts with outputs off
    hal.AssertOutputs("___");
}

TEST_F(LightTests, LightsOnAndOff)
{
    AssertLights("___");

    Lights_set_range(0, 2);
    AssertLights("###");

    Lights_off();
    AssertLights("___");
}

TEST_F(LightTests, Set)
{
    Lights_set(0,1,0);
    AssertLights("_#_");

    Lights_set(1,0,1);
    AssertLights("#_#");
}

TEST_F(LightTests, SetRange)
{
    Lights_set_range(0, 0);
    AssertLights("#__");

    Lights_set_range(1, 1);
    AssertLights("_#_");

    Lights_set_range(2, 2);
    AssertLights("__#");

    Lights_set_range(0, 1);
    AssertLights("##_");

    Lights_set_range(1, 2);
    AssertLights("_##");

    Lights_set_range(0, 2);
    AssertLights("###");
}

TEST_F(LightTests, SetRange_FromGreaterThanTo)
{
    Lights_set_range(2, 0);
    AssertLights("___");
}

TEST_F(LightTests, SetRange_BeyondEnd)
{
    Lights_set_range(1, 77);
    AssertLights("_##");
}

TEST_F(LightTests, SetRange_InvalidStartEnd)
{
    Lights_set_range(10, 77);
    AssertLights("___");
}
