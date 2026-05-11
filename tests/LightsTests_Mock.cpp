extern "C"
{
#include "Lights.h"
}
#include "doubles/MockHal.hpp"
#include "gtest_helpers.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing; // NOTE! This allows to omit "::testing" everywhere.

// lots of good info here: https://google.github.io/googletest/gmock_for_dummies.html

TEST(LightTests_Mock, Set_AnyOrder)
{
    MockHal hal;

    EXPECT_CALL(hal, SetOutput(0, true)).Times(1);
    EXPECT_CALL(hal, SetOutput(1, false)).Times(1);
    EXPECT_CALL(hal, SetOutput(2, false)).Times(1);

    Lights_set(1,0,0);
}

// If you want to ensure order of calls. Overkill for this output setting example
// but sometimes it does matter. Order matters in most communications protocols.
// see https://google.github.io/googletest/gmock_for_dummies.html#OrderedCalls
TEST(LightTests_Mock, Set_InOrder)
{
    MockHal hal;

    InSequence seq; // this causes expectation order below to matter

    EXPECT_CALL(hal, SetOutput(0, true)).Times(1);
    EXPECT_CALL(hal, SetOutput(1, false)).Times(1);
    EXPECT_CALL(hal, SetOutput(2, false)).Times(1);

    Lights_set(1,0,0);
}

// we create a test fixture so that FakeHal is constructed fresh for every test.
// we also put a small helper in there.
class LightTests_MockFixture : public ::testing::Test
{
protected:
    MockHal hal;

    void ExpectSetOutput(uint8_t index, bool state)
    {
        EXPECT_CALL(hal, SetOutput(index, state)).Times(1).RetiresOnSaturation();
    }
};

TEST_F(LightTests_MockFixture, Set)
{
    ExpectSetOutput(0, false);
    ExpectSetOutput(1, true);
    ExpectSetOutput(2, false);

    Lights_set(0,1,0);
}

TEST_F(LightTests_MockFixture, MultipleSets)
{
    ExpectSetOutput(0, false);
    ExpectSetOutput(1, true);
    ExpectSetOutput(2, false);
    Lights_set(0,1,0);
    ASSERT_TRUE(Mock::VerifyAndClearExpectations(&hal));

    // NOTE! Technically, adding expectations after mock exercised is undefined behavior.
    // However, googletest has many unit tests that guarantee just this in **single threaded code**.
    // I've been relying on this approach for almost a decade with no issues. Many others as well.
    // https://github.com/google/googletest/issues/4846
    // https://github.com/google/googletest/issues/2828

    // All bets are off in multi threaded code, but I try to not to use multithreading in tests anyway.
    // If you are concerned, you can reconstruct the MockHal object here by using std::unique_ptr<MockHal>() in below tests.

    ExpectSetOutput(0, false);
    ExpectSetOutput(1, true);
    ExpectSetOutput(2, true);
    Lights_set(0,1,1);
    ASSERT_TRUE(Mock::VerifyAndClearExpectations(&hal));
}

////////////////////////////////////////////// SAFER OPTION //////////////////////////////////////////////////

// Below shows a safe alternative to potential (but unlikely) undefined behavior in
// https://github.com/google/googletest/issues/4846
// https://github.com/google/googletest/issues/2828


#include <memory> // you should normally put this at the top of your file. Quick and dirty here for example.
class LightTests_MockFixture_NoUB : public ::testing::Test
{
protected:
    std::unique_ptr<MockHal> hal;

    LightTests_MockFixture_NoUB() 
    : hal(std::make_unique<MockHal>()) // C++ member initialization
    {
    }

    void ExpectSetOutput(uint8_t index, bool state)
    {
        EXPECT_CALL(*hal, SetOutput(index, state)).Times(1).RetiresOnSaturation();
    }

    void VerifyAndClearExpectations()
    {
        ASSERT_TRUE(Mock::VerifyAndClearExpectations(&hal));

        // Reset mock expectations
        // This is safe. No Undefined Behavior.
        // See https://github.com/google/googletest/issues/2828#issuecomment-717935800
        hal.reset(); // causes existing MockHal object destruction.
        hal = std::make_unique<MockHal>();
    }
};

TEST_F(LightTests_MockFixture_NoUB, MultipleSets)
{
    ExpectSetOutput(0, false);
    ExpectSetOutput(1, true);
    ExpectSetOutput(2, false);
    Lights_set(0,1,0);
    VerifyAndClearExpectations();
    RETURN_IF_FAILURE();

    ExpectSetOutput(0, false);
    ExpectSetOutput(1, true);
    ExpectSetOutput(2, true);
    Lights_set(0,1,1);
    VerifyAndClearExpectations();
    RETURN_IF_FAILURE();
}
