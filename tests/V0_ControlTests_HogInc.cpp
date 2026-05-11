// "Hand of God" testing.
// ❌ Includes source file (try to avoid this if you can).
// ⏩ We will improve later.

extern "C"
{
#include "V0/Control.c" // NOTE!!! INCLUDING SOURCE FILE TO ACCESS PRIVATES!!!
}
#include "doubles/FakeHal.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// benefit of "Hand of God" testing is that we can directly jump to any state to test
// so let's start by testing ON3
TEST(V0_ControlTests, On3ToOn2)
{
    // ARRANGE
    FakeHal hal;
    state = V0_StateId_ON3;
    // REMEMBER! ON3 enter didn't actually run. We just forced the state value.
    // we can't test for state of outputs yet.

    // ACT
    V0_Control_handle_event(V0_EventId_DIM);

    // ASSERT
    ASSERT_EQ(state, V0_StateId_ON2);
    ASSERT_THAT(hal.outputs, ::testing::ElementsAre(1, 1, 0));
}

TEST(V0_ControlTests, On2ToOn1)
{
    // ARRANGE
    FakeHal hal;
    state = V0_StateId_ON2;

    // ACT
    V0_Control_handle_event(V0_EventId_DIM);

    // ASSERT
    ASSERT_EQ(state, V0_StateId_ON1);
    ASSERT_THAT(hal.outputs, ::testing::ElementsAre(1, 0, 0));
}

// Option 1. Directly set count.
TEST(V0_ControlTests, On2ToOn3_copy_enter_effect)
{
    // ARRANGE
    FakeHal hal;
    state = V0_StateId_ON2;
    count = 0;
    // NOTE! Manually copy effect of ON2 enter to reset count. Don't forget or test order matters!
    // While this works, it is pretty gross. We are copying fsm logic into our tests.
    // Even more problematic, what if FSM code forgot to do this??? False confidence from passing test!
    // Without setting this, try running with gtest flags GTEST_REPEAT=2 GTEST_FILTER=V0*_copy_enter_effect.
    // You can also periodically see a failure if you shuffle the test order: GTEST_BRIEF=1 GTEST_SHUFFLE=1 cmake --build . --target run_tests
    // In other test files, we will use google test fixtures to safely reset state before each test so we can't forget.

    // And while I'm complaining, `count` is so generically named that it's unclear which file defines it.
    // Is `count` defined in a gtest fixture? Or is it defined in Control.c? Can get confusing in larger designs.

    // ACT / ASSERT
    V0_Control_handle_event(V0_EventId_INC);
    ASSERT_EQ(state, V0_StateId_ON2);
    V0_Control_handle_event(V0_EventId_INC);
    ASSERT_EQ(state, V0_StateId_ON2);
    V0_Control_handle_event(V0_EventId_INC);

    // ASSERT
    ASSERT_EQ(state, V0_StateId_ON3);
    ASSERT_THAT(hal.outputs, ::testing::ElementsAre(1, 1, 1));
}

// Because we included the .c file, we have access to the `enter_ON2()` function.
// Instead of forcing state directly like above tests, we can call `enter_ON2()`.
// Pro: it will set state properly for ON2 and run its entry code.
// Con: we are really getting tightly coupled to implementation. Fine for small designs.
TEST(V0_ControlTests, On2ToOn3_use_enter_code)
{
    // ARRANGE
    // -----------------------------------------------------------------
    FakeHal hal;
    count = 99;  // corrupt count to verify On2 enter resets it
    enter_ON2(); // call private fsm function
    ASSERT_EQ(count, 0) << "ON2 enter should have reset count";

    // ACT / ASSERT
    V0_Control_handle_event(V0_EventId_INC);
    ASSERT_EQ(state, V0_StateId_ON2);
    V0_Control_handle_event(V0_EventId_INC);
    ASSERT_EQ(state, V0_StateId_ON2);
    V0_Control_handle_event(V0_EventId_INC);

    // ASSERT
    ASSERT_EQ(state, V0_StateId_ON3);
    ASSERT_THAT(hal.outputs, ::testing::ElementsAre(1, 1, 1));
}

// don't forget to add tests for events that should be ignored in states
TEST(V0_ControlTests, On3_ignore_inc_events)
{
    // ARRANGE
    // -----------------------------------------------------------------
    FakeHal hal;
    enter_ON3();
    ASSERT_THAT(hal.outputs, ::testing::ElementsAre(1, 1, 1));

    // ACT
    V0_Control_handle_event(V0_EventId_INC);

    // ASSERT
    ASSERT_EQ(state, V0_StateId_ON3);
    ASSERT_THAT(hal.outputs, ::testing::ElementsAre(1, 1, 1));
}

/////////////////////////////////////////////////////////////////////////////
// AI GENERATED TESTS FOLLOW
/////////////////////////////////////////////////////////////////////////////

TEST(V0_ControlTests, Off_ignore_dim_events)
{
    // ARRANGE
    FakeHal hal;
    enter_OFF();
    ASSERT_THAT(hal.outputs, ::testing::ElementsAre(0, 0, 0));

    // ACT
    V0_Control_handle_event(V0_EventId_DIM);

    // ASSERT
    ASSERT_EQ(state, V0_StateId_OFF);
    ASSERT_THAT(hal.outputs, ::testing::ElementsAre(0, 0, 0));
}

TEST(V0_ControlTests, OffToOn1)
{
    // ARRANGE
    FakeHal hal;
    enter_OFF();

    // ACT
    V0_Control_handle_event(V0_EventId_INC);

    // ASSERT
    ASSERT_EQ(state, V0_StateId_ON1);
    ASSERT_THAT(hal.outputs, ::testing::ElementsAre(1, 0, 0));
}

TEST(V0_ControlTests, On1ToOff)
{
    // ARRANGE
    FakeHal hal;
    enter_ON1();

    // ACT
    V0_Control_handle_event(V0_EventId_DIM);

    // ASSERT
    ASSERT_EQ(state, V0_StateId_OFF);
    ASSERT_THAT(hal.outputs, ::testing::ElementsAre(0, 0, 0));
}

TEST(V0_ControlTests, On1ToOn2)
{
    // ARRANGE
    FakeHal hal;
    enter_ON1();

    // ACT
    V0_Control_handle_event(V0_EventId_INC);

    // ASSERT
    ASSERT_EQ(state, V0_StateId_ON2);
    ASSERT_THAT(hal.outputs, ::testing::ElementsAre(1, 1, 0));
}

// Verify the guard [count >= 3] does not fire early
TEST(V0_ControlTests, On2_stays_on_inc_below_threshold)
{
    // ARRANGE
    FakeHal hal;
    enter_ON2();
    ASSERT_EQ(count, 0) << "ON2 enter should have reset count";

    // ACT / ASSERT
    V0_Control_handle_event(V0_EventId_INC); // count = 1
    ASSERT_EQ(state, V0_StateId_ON2);
    V0_Control_handle_event(V0_EventId_INC); // count = 2
    ASSERT_EQ(state, V0_StateId_ON2);
    // one more INC would trigger transition — stop here to test the boundary
}
