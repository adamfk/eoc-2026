// ✅ This test file uses helpers so that we can efficiently specify Natural Flow tests and avoid huge repetition.
// ✅ Outputs more useful stack trace information.
// ⏩ Next alternative approach outputs better caller trace information.

extern "C"
{
#include "V2/Control.h"
}
#include "FakeHal.hpp"
#include "gtest_helpers.hpp"
#include <gtest/gtest.h>
#include <functional>

class V2_ControlTests2 : public ::testing::Test
{
protected:
    FakeHal hal;

    V2_ControlTests2()
    {
        V2_Control_setup();
    }

    void SetUp() override
    {
        // runs after constructor.
        // you can use ASSERT_ macros here.
        AssertState(V2_ControlSm_StateId_OFF);
        RETURN_IF_FAILURE();
        AssertLights("___");
        RETURN_IF_FAILURE();
    }

    void AssertLights(std::string_view lights)
    {
        SCOPED_TRACE_FUNC(lights);
        hal.AssertOutputs(lights);
    }

    void AssertState(V2_ControlSm_StateId expected_state)
    {
        SCOPED_TRACE_FUNC(expected_state);

        const V2_ControlSm_StateId actual_state = V2_Control_get_state();

        if (expected_state != actual_state)
        {
            FAIL() << "Expected state `" << V2_ControlSm_state_id_to_string(expected_state)
                   << "`, but was in state `" << V2_ControlSm_state_id_to_string(actual_state) << "`.";
        }
    }

    void IncPress()
    {
        V2_Control_inc_press();
    }

    void DimPress()
    {
        V2_Control_dim_press();
    }

    void OffPress()
    {
        V2_Control_off_press();
    }

    void ReachOn1()
    {
        SCOPED_TRACE_FUNC();

        IncPress();
        AssertState(V2_ControlSm_StateId_ON1);
        RETURN_IF_FAILURE();
        AssertLights("#__");
        RETURN_IF_FAILURE();
    }

    void ReachOn2()
    {
        SCOPED_TRACE_FUNC();
        ReachOn1();
        RETURN_IF_FAILURE();

        IncPress();
        AssertState(V2_ControlSm_StateId_ON2);
        RETURN_IF_FAILURE();
        AssertLights("##_");
        RETURN_IF_FAILURE();
    }

    void ReachOn3()
    {
        SCOPED_TRACE_FUNC();
        ReachOn2();
        RETURN_IF_FAILURE();

        for (size_t i = 0; i < 3 - 1; i++)
        {
            IncPress();
            AssertState(V2_ControlSm_StateId_ON2);
            RETURN_IF_FAILURE();
            AssertLights("##_");
            RETURN_IF_FAILURE();
        }

        IncPress();
        AssertState(V2_ControlSm_StateId_ON3);
        RETURN_IF_FAILURE();
        AssertLights("###");
        RETURN_IF_FAILURE();
    }
};

//////////////////////////////////// START OF TESTS ////////////////////////////////////

TEST_F(V2_ControlTests2, Off_IncTo_On1)
{
    SCOPED_TRACE_FUNC(); // helps navigate directly to test body from failure logs. Optional.
    ReachOn1();
}

TEST_F(V2_ControlTests2, Off_ignores_dim__and_off)
{
    DimPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
    RETURN_IF_FAILURE();

    OffPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

TEST_F(V2_ControlTests2, On1_DimTo_Off)
{
    ReachOn1();
    RETURN_IF_FAILURE();

    DimPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

TEST_F(V2_ControlTests2, On1_IncTo_On2)
{
    ReachOn2();
}

TEST_F(V2_ControlTests2, On2_DimTo_On1)
{
    ReachOn2();
    RETURN_IF_FAILURE();

    DimPress();
    AssertState(V2_ControlSm_StateId_ON1);
    AssertLights("#__");
}

TEST_F(V2_ControlTests2, On2_IncTo_On3)
{
    ReachOn3();
    RETURN_IF_FAILURE();
}

TEST_F(V2_ControlTests2, On3_DimTo_On2)
{
    ReachOn3();
    RETURN_IF_FAILURE();

    DimPress();
    AssertState(V2_ControlSm_StateId_ON2);
    AssertLights("##_");
}

TEST_F(V2_ControlTests2, On3_ignore_inc)
{
    ReachOn3();
    RETURN_IF_FAILURE();

    IncPress();
    AssertState(V2_ControlSm_StateId_ON3);
    AssertLights("###");
}

////////////////////////////////

TEST_F(V2_ControlTests2, CodeCoverageForStateMachineHelpers)
{
    // if you don't care about testing these helper functions

    for (size_t i = 0; i <= V2_ControlSm_EventIdCount; i++)
    {
        (void)V2_ControlSm_event_id_to_string((V2_ControlSm_EventId)i);
    }

    for (size_t i = 0; i <= V2_ControlSm_StateIdCount; i++)
    {
        (void)V2_ControlSm_state_id_to_string((V2_ControlSm_StateId)i);
        (void)V2_ControlSm_get_parent_id((V2_ControlSm_StateId)i);
    }
}

//////////////////////////

TEST_F(V2_ControlTests2, On1_OffTo_Off)
{
    ReachOn1();
    RETURN_IF_FAILURE();

    OffPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

TEST_F(V2_ControlTests2, On2_OffTo_Off)
{
    ReachOn2();
    RETURN_IF_FAILURE();

    OffPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

TEST_F(V2_ControlTests2, On3_OffTo_Off)
{
    ReachOn3();
    RETURN_IF_FAILURE();

    OffPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

///////////////////////////

TEST_F(V2_ControlTests2, OnGroup_OffTo_Off)
{
    std::vector<std::function<void()>> reach_methods = {
        [this]{ ReachOn1(); },
        [this]{ ReachOn2(); },
        [this]{ ReachOn3(); }
    };

    for (auto& reach_method : reach_methods)
    {
        reach_method();
        RETURN_IF_FAILURE();

        OffPress();
        AssertState(V2_ControlSm_StateId_OFF);
        AssertLights("___");
    }
}

///////////////////////////

/**
 * This test shows how you can use a StateSmith feature to easily ensure that you
 * don't miss a single substate as a design evolves.
 */
TEST_F(V2_ControlTests2, OnGroup_OffTo_Off_ForHugeDesign)
{
    SCOPED_TRACE_FUNC(); // helps navigate directly to test body from failure logs

    std::map<V2_ControlSm_StateId, std::function<void()>> state_to_reach_method_map = {
        {V2_ControlSm_StateId_ON1, [this]{ ReachOn1(); }},
        {V2_ControlSm_StateId_ON2, [this]{ ReachOn2(); }},
        {V2_ControlSm_StateId_ON3, [this]{ ReachOn3(); }},
    };

    // Uses StateSmith specific functionality https://github.com/StateSmith/StateSmith/issues/538
    const size_t first_substate = V2_ControlSm_StateId_ON_GROUP + 1;
    const size_t last_substate = V2_ControlSm_ON_GROUP_SubtreeEndId;
    
    for (size_t i = first_substate; i <= last_substate; i++)
    {
        const V2_ControlSm_StateId state_id = (V2_ControlSm_StateId)i;

        // find reach method or assert
        ASSERT_TRUE(state_to_reach_method_map.contains(state_id)) << "No reach method found for state '" << V2_ControlSm_state_id_to_string(state_id) << "'";
        std::function<void()>& reach_method = state_to_reach_method_map.at((V2_ControlSm_StateId)i);

        reach_method();
        RETURN_IF_FAILURE();

        OffPress();
        AssertState(V2_ControlSm_StateId_OFF);
        AssertLights("___");
    }
}
