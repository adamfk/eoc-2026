// We import C header inside an extern C block so linker knows
// how to link C and C++ code. Could also do inside of Lights.h.
extern "C"
{
#include "Lights.h"
}
#include "FakeHal.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h> // for ::testing::ElementsAre

TEST(LightTestsBasic, Set)
{
    FakeHal hal; // this calls FakeHal constructor to set it up.

    Lights_set(1,0,0); // C code
    ASSERT_EQ(hal.outputs[0], true);
    ASSERT_EQ(hal.outputs[1], false);
    ASSERT_EQ(hal.outputs[2], false);

    // hal's destructor automatically called when it goes out of scope.
}

TEST(LightTestsBasic, Set_VerifyWithHelper)
{
    FakeHal hal; // this calls FakeHal constructor to set it up.

    Lights_set(0,1,0);
    ASSERT_THAT(hal.outputs, ::testing::ElementsAre(0, 1, 0));  // gtest helper

    Lights_set(1,0,1);
    ASSERT_THAT(hal.outputs, ::testing::ElementsAre(1, 0, 1));  // gtest helper
}

// enable below to explore C++ array indexing options and safety
#if 0
TEST(OutOfBoundsExample, ExceptionAllowsTestsToContinue)
{
    FakeHal hal;
    hal.SetOutput(3, true); // off by 1 error! Out of bounds.
    // SetOutput throws an exception which will allows other tests to continue.
}

TEST(OutOfBoundsExample, BadAccessSegfaults)
{
    FakeHal hal;
    hal.outputs[3] = true;  // off by 1 error! Out of bounds.
    // Will give undefined behavior on bad index. Hopefully crash right away.
}
#endif
