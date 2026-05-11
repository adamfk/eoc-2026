// "Hand of God" testing.
// ❌ Requires special UNIT_TESTING accessors (not too bad, but not great either).
// ⏩ We will improve later.

extern "C"
{
    #include "V1/Control.h"
}
#include "doubles/FakeHal.hpp"
#include "gtest_helpers.hpp"
#include <gtest/gtest.h>

class V1_ControlTests : public ::testing::Test
{
protected:
    FakeHal hal;  // this gets reconstructed every test. Automatic cleanup and setup.

    V1_ControlTests()
    {
        V1_Control_setup(); // this helps ensures clean setup every test
    }

    // We assert lights state in one place.
    // If lights mapping to hardware changes, we can update this one helper.
    // Don't sleep on this tip! It makes maintenance much much easier. 
    void AssertLights(std::string_view lights)
    {
        hal.AssertOutputs(lights);
    }

    void AssertState(V1_StateId expected_state)
    {
        const V1_StateId actual_state = V1_Control_get_state();
        ASSERT_EQ(expected_state, actual_state); // we will improve later
    }
};

// benefit of "Hand of God" testing is that we can directly jump to any state to test
// so let's start by testing ON3
TEST_F(V1_ControlTests, On3ToOn2)
{
    // ARRANGE
    UNIT_TESTING_V1_Control_set_state(V1_StateId_ON3);
    // REMEMBER! ON3 enter didn't actually run. We just forced the state value.
    // we can't test for state of outputs yet.

    // ACT
    V1_Control_handle_event(V1_EventId_DIM);

    // ASSERT
    AssertState(V1_StateId_ON2);
    AssertLights("##_");
}

TEST_F(V1_ControlTests, On2ToOn1)
{
    // ARRANGE
    UNIT_TESTING_V1_Control_set_state(V1_StateId_ON2);

    // ACT
    V1_Control_handle_event(V1_EventId_DIM);

    // ASSERT
    AssertState(V1_StateId_ON1);
    AssertLights("#__");
}

// Option 1. Directly set count.
TEST_F(V1_ControlTests, On2ToOn3_copy_enter_effect)
{
    // ARRANGE
    UNIT_TESTING_V1_Control_set_state(V1_StateId_ON2);
    UNIT_TESTING_V1_Control_set_count(0);
    // NOTE! Manually copy effect of ON2 enter to reset count. Don't forget this or you rely on tests that ran before!
    // While this works, it is pretty gross. We are copying fsm logic into our tests.
    // We technically don't need this line because our test fixture clears count before each test,
    // but what if ON2 enter had a different entry effect other just resetting count?

    // Even more problematic, what if FSM code forgot to do this??? False confidence from test!

    // ACT / ASSERT
    V1_Control_handle_event(V1_EventId_INC);
    AssertState(V1_StateId_ON2);
    V1_Control_handle_event(V1_EventId_INC);
    AssertState(V1_StateId_ON2);
    V1_Control_handle_event(V1_EventId_INC);

    // ASSERT
    AssertState(V1_StateId_ON3);
    AssertLights("###");
}

// we didn't include the .c file, so we can't directly call `enter_ON2()` function.
// instead, we need to travel into ON2 and to ON3 to ensure count was cleared
TEST_F(V1_ControlTests, On2EntryClearsCount)
{
    // ARRANGE
    UNIT_TESTING_V1_Control_set_state(V1_StateId_ON1);
    UNIT_TESTING_V1_Control_set_count(99); // corrupt count

    // Get to ON2
    V1_Control_handle_event(V1_EventId_INC);
    AssertState(V1_StateId_ON2);
    AssertLights("##_"); // we can now assert some ON2 entry behavior

    // Get to ON3
    V1_Control_handle_event(V1_EventId_INC);
    AssertState(V1_StateId_ON2);
    V1_Control_handle_event(V1_EventId_INC);
    AssertState(V1_StateId_ON2);
    V1_Control_handle_event(V1_EventId_INC);

    // ASSERT
    AssertState(V1_StateId_ON3);
    AssertLights("###");
}

/////////////////////////////////////////////////////////////////////////////
// AI GENERATED TESTS FOLLOW
/////////////////////////////////////////////////////////////////////////////

TEST_F(V1_ControlTests, Off_ignore_dim_events)
{
    // ARRANGE
    UNIT_TESTING_V1_Control_set_state(V1_StateId_OFF);

    // ACT
    V1_Control_handle_event(V1_EventId_DIM);

    // ASSERT
    AssertState(V1_StateId_OFF);
    AssertLights("___");
}

TEST_F(V1_ControlTests, OffToOn1)
{
    // ARRANGE
    UNIT_TESTING_V1_Control_set_state(V1_StateId_OFF);

    // ACT
    V1_Control_handle_event(V1_EventId_INC);

    // ASSERT
    AssertState(V1_StateId_ON1);
    AssertLights("#__");
}

TEST_F(V1_ControlTests, On1ToOff)
{
    // ARRANGE
    UNIT_TESTING_V1_Control_set_state(V1_StateId_ON1);

    // ACT
    V1_Control_handle_event(V1_EventId_DIM);

    // ASSERT
    AssertState(V1_StateId_OFF);
    AssertLights("___");
}

TEST_F(V1_ControlTests, On1ToOn2)
{
    // ARRANGE
    UNIT_TESTING_V1_Control_set_state(V1_StateId_ON1);

    // ACT
    V1_Control_handle_event(V1_EventId_INC);

    // ASSERT
    AssertState(V1_StateId_ON2);
    AssertLights("##_");
}

// Verify the guard [count >= 3] does not fire early
TEST_F(V1_ControlTests, On2_stays_on_inc_below_threshold)
{
    // ARRANGE
    UNIT_TESTING_V1_Control_set_state(V1_StateId_ON2);
    UNIT_TESTING_V1_Control_set_count(0);

    // ACT / ASSERT
    V1_Control_handle_event(V1_EventId_INC); // count = 1
    AssertState(V1_StateId_ON2);
    V1_Control_handle_event(V1_EventId_INC); // count = 2
    AssertState(V1_StateId_ON2);
    // stopping here — transition at count=3 is covered by On2ToOn3_copy_enter_effect
}

TEST_F(V1_ControlTests, On3_ignore_inc_events)
{
    // ARRANGE
    UNIT_TESTING_V1_Control_set_state(V1_StateId_ON3);

    // ACT
    V1_Control_handle_event(V1_EventId_INC);

    // ASSERT
    AssertState(V1_StateId_ON3);
    // NOTE: can't assert lights here — ON3 enter never ran, outputs are in unknown state
}
