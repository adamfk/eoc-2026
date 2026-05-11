// ❌ This file is intentionally repetitive. This is the "naive" way to implement Natural Flow testing.
// Try changing the state machine implementation and see how annoying it is to update the tests.
// ⏩ Improved in V2 tests.

extern "C"
{
    #include "V1/Control.h"
}
#include "doubles/FakeHal.hpp"
#include "gtest_helpers.hpp"
#include <gtest/gtest.h>

class V1_ControlTests_NaturalFlow : public ::testing::Test
{
protected:
    FakeHal hal;  // this gets reconstructed every test. Automatic cleanup and setup.

    V1_ControlTests_NaturalFlow()
    {
        V1_Control_setup();
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

    // not needed, but helps when you write it a lot
    void IncPress()
    {
        V1_Control_handle_event(V1_EventId_INC);
    }

    void DimPress()
    {
        V1_Control_handle_event(V1_EventId_DIM);
    }
};

TEST_F(V1_ControlTests_NaturalFlow, StartsOff)
{
    AssertLights("___");
    AssertState(V1_StateId_OFF);
}

TEST_F(V1_ControlTests_NaturalFlow, OffIgnoresDim)
{
    AssertLights("___");
    AssertState(V1_StateId_OFF);

    DimPress();
    AssertLights("___");
    AssertState(V1_StateId_OFF);
}

TEST_F(V1_ControlTests_NaturalFlow, OffIncToOn1)
{
    AssertLights("___");
    AssertState(V1_StateId_OFF);

    IncPress();
    AssertLights("#__");
    AssertState(V1_StateId_ON1);
}

TEST_F(V1_ControlTests_NaturalFlow, On1IncToOn2)
{
    AssertLights("___");
    AssertState(V1_StateId_OFF);

    IncPress();
    AssertLights("#__");
    AssertState(V1_StateId_ON1);

    IncPress();
    AssertLights("##_");
    AssertState(V1_StateId_ON2);
}

TEST_F(V1_ControlTests_NaturalFlow, On1DimToOff)
{
    AssertLights("___");
    AssertState(V1_StateId_OFF);

    IncPress();
    AssertLights("#__");
    AssertState(V1_StateId_ON1);

    DimPress();
    AssertLights("___");
    AssertState(V1_StateId_OFF);
}

TEST_F(V1_ControlTests_NaturalFlow, On2DimToOn1)
{
    AssertLights("___");
    AssertState(V1_StateId_OFF);

    IncPress();
    AssertLights("#__");
    AssertState(V1_StateId_ON1);

    IncPress();
    AssertLights("##_");
    AssertState(V1_StateId_ON2);

    DimPress();
    AssertLights("#__");
    AssertState(V1_StateId_ON1);
}

TEST_F(V1_ControlTests_NaturalFlow, On2IncToOn3)
{
    AssertLights("___");
    AssertState(V1_StateId_OFF);

    IncPress();
    AssertLights("#__");
    AssertState(V1_StateId_ON1);

    IncPress();
    AssertLights("##_");
    AssertState(V1_StateId_ON2);

    for (size_t i = 0; i < 3 - 1; i++)
    {
        IncPress();
        AssertLights("##_");
        AssertState(V1_StateId_ON2);
    }
    
    IncPress();
    AssertLights("###");
    AssertState(V1_StateId_ON3);
}

TEST_F(V1_ControlTests_NaturalFlow, On3DimToOn2)
{
    AssertLights("___");
    AssertState(V1_StateId_OFF);

    IncPress();
    AssertLights("#__");
    AssertState(V1_StateId_ON1);

    IncPress();
    AssertLights("##_");
    AssertState(V1_StateId_ON2);

    for (size_t i = 0; i < 3 - 1; i++)
    {
        IncPress();
        AssertLights("##_");
        AssertState(V1_StateId_ON2);
    }
    
    IncPress();
    AssertLights("###");
    AssertState(V1_StateId_ON3);

    DimPress();
    AssertLights("##_");
    AssertState(V1_StateId_ON2);
}

///////////// AI GENERATED TESTS FOLLOW ////////////////

// ON3 has no INC transition defined — should be ignored
TEST_F(V1_ControlTests_NaturalFlow, On3IgnoresInc)
{
    IncPress(); // OFF -> ON1
    IncPress(); // ON1 -> ON2
    for (size_t i = 0; i < 3; i++) IncPress(); // ON2 -> ON3 (count guard)

    AssertLights("###");
    AssertState(V1_StateId_ON3);

    IncPress(); // no transition — should stay
    AssertLights("###");
    AssertState(V1_StateId_ON3);
}

// Re-entering ON2 must reset count, so 3 more INC presses are needed again
TEST_F(V1_ControlTests_NaturalFlow, On2CountResetsOnReentry)
{
    IncPress(); // OFF -> ON1
    IncPress(); // ON1 -> ON2

    // Use 2 of the 3 required INC presses in ON2
    IncPress();
    IncPress();
    AssertLights("##_");
    AssertState(V1_StateId_ON2);

    // Leave and come back — count should reset
    DimPress(); // ON2 -> ON1
    IncPress(); // ON1 -> ON2 (re-entry, count = 0)
    AssertLights("##_");
    AssertState(V1_StateId_ON2);

    // Need 3 fresh presses now, not just 1
    IncPress();
    IncPress();
    AssertLights("##_");
    AssertState(V1_StateId_ON2); // still ON2, guard not yet satisfied

    IncPress();
    AssertLights("###");
    AssertState(V1_StateId_ON3);
}

// After ON3->ON2->ON3, count must again require 3 INC presses
TEST_F(V1_ControlTests_NaturalFlow, On3On2On3CountResetsAgain)
{
    IncPress(); // OFF -> ON1
    IncPress(); // ON1 -> ON2
    for (size_t i = 0; i < 3; i++) IncPress(); // -> ON3

    AssertState(V1_StateId_ON3);

    DimPress(); // ON3 -> ON2 (re-entry, count = 0)
    AssertLights("##_");
    AssertState(V1_StateId_ON2);

    // 2 presses: still ON2
    IncPress();
    IncPress();
    AssertLights("##_");
    AssertState(V1_StateId_ON2);

    // 3rd press: transition fires
    IncPress();
    AssertLights("###");
    AssertState(V1_StateId_ON3);
}

// Guard is strictly count >= 3: exactly 3 INC in ON2 is the minimum
TEST_F(V1_ControlTests_NaturalFlow, On2GuardExactlyThreePresses)
{
    IncPress(); // OFF -> ON1
    IncPress(); // ON1 -> ON2

    // 2 presses: must NOT transition
    IncPress();
    IncPress();
    AssertLights("##_");
    AssertState(V1_StateId_ON2);

    // 3rd press: must transition
    IncPress();
    AssertLights("###");
    AssertState(V1_StateId_ON3);
}

// Full round-trip: OFF -> ON1 -> ON2 -> ON3 -> ON2 -> ON1 -> OFF
TEST_F(V1_ControlTests_NaturalFlow, FullRoundTrip)
{
    AssertState(V1_StateId_OFF);

    IncPress();
    AssertState(V1_StateId_ON1);

    IncPress();
    AssertState(V1_StateId_ON2);

    for (size_t i = 0; i < 3; i++) IncPress();
    AssertState(V1_StateId_ON3);

    DimPress();
    AssertState(V1_StateId_ON2);
    AssertLights("##_");

    DimPress();
    AssertState(V1_StateId_ON1);
    AssertLights("#__");

    DimPress();
    AssertState(V1_StateId_OFF);
    AssertLights("___");
}
