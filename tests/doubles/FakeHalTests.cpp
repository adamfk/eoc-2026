#include "FakeHal.hpp"
#include <gtest/gtest.h>

// these tests help ensure that our FakeHal works as intended

TEST(FakeHal, ThrowAsNoGlobalHalYet)
{
    EXPECT_THROW(Hal_set_output(0, true), std::runtime_error);
}

TEST(FakeHal, NormalUsage)
{
    FakeHal fake_hal;

    Hal_set_output(0, true);
    ASSERT_EQ("#__", fake_hal.GetOutputsString());

    Hal_set_output(1, true);
    ASSERT_EQ("##_", fake_hal.GetOutputsString());

    Hal_set_output(2, true);
    ASSERT_EQ("###", fake_hal.GetOutputsString());

    Hal_set_output(2, false);
    ASSERT_EQ("##_", fake_hal.GetOutputsString());

    Hal_set_output(0, false);
    ASSERT_EQ("_#_", fake_hal.GetOutputsString());

    Hal_set_output(1, false);
    ASSERT_EQ("___", fake_hal.GetOutputsString());
}

TEST(FakeHal, ThrowOnInvalidRange)
{
    FakeHal fake_hal;
    EXPECT_THROW(Hal_set_output(Hal_MAX_OUTPUTS + 1, true), std::out_of_range);
}

TEST(FakeHal, ThrowOnMultipleInstances)
{
    FakeHal fake_hal;
    EXPECT_THROW(FakeHal kaboom;, std::runtime_error);
}
