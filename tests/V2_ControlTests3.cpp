// ✅ This provides the best stack trace information, although I find it a bit slower to write as all helper methods are wrapped.

// Shows an alternative to using SCOPED_TRACE_FUNC() and RETURN_IF_FAILURE() everywhere.
// This provides calling site stack trace information.
// This can be helpful if a test or method uses the same assertion helper method multiple times.
// With `SCOPED_TRACE_FUNC()` approach, you won't be able to tell calling site.

// Gives a failure stack trace like this:
//      tests/V2_ControlTests3.cpp:41: Failure
//      Failed
//      Expected state `ON1`, but was in state `OFF`.
//      Google Test trace:
//      tests/V2_ControlTests3.cpp:64: From ReachOn1, call: AssertState(V2_ControlSm_StateId_ON1)
//      tests/V2_ControlTests3.cpp:225: From TestBody, call: reach_method()

extern "C"
{
#include "V2/Control.h"
}
#include "FakeHal.hpp"
#include "gtest_helpers.hpp"
#include <gtest/gtest.h>
#include <functional>

class V2_ControlTests3 : public ::testing::Test
{
protected:
    FakeHal hal;

    V2_ControlTests3()
    {
        V2_Control_setup();
    }

    void SetUp() override
    {
        // runs after constructor.
        // you can use ASSERT_ macros here.
        ASSERT_CALL(AssertState(V2_ControlSm_StateId_OFF));
        ASSERT_CALL(AssertLights("___"));
    }

    void AssertLights(std::string_view lights)
    {
        ASSERT_CALL(hal.AssertOutputs(lights));
    }

    void AssertState(V2_ControlSm_StateId expected_state)
    {
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
        IncPress();
        ASSERT_CALL(AssertState(V2_ControlSm_StateId_ON1));
        ASSERT_CALL(AssertLights("#__"));
    }

    void ReachOn2()
    {
        ASSERT_CALL(ReachOn1());

        IncPress();
        ASSERT_CALL(AssertState(V2_ControlSm_StateId_ON2));
        ASSERT_CALL(AssertLights("##_"));
    }

    void ReachOn3()
    {
        ASSERT_CALL(ReachOn2());

        for (size_t i = 0; i < 3 - 1; i++)
        {
            IncPress();
            ASSERT_CALL(AssertState(V2_ControlSm_StateId_ON2));
            ASSERT_CALL(AssertLights("##_"));
        }

        IncPress();
        ASSERT_CALL(AssertState(V2_ControlSm_StateId_ON3));
        ASSERT_CALL(AssertLights("###"));
    }
};

//////////////////////////////////// START OF TESTS ////////////////////////////////////

TEST_F(V2_ControlTests3, Off_IncTo_On1)
{
    ASSERT_CALL(ReachOn1());
}

TEST_F(V2_ControlTests3, Off_ignores_dim__and_off)
{
    DimPress();
    ASSERT_CALL(AssertState(V2_ControlSm_StateId_OFF));
    ASSERT_CALL(AssertLights("___"));

    OffPress();
    ASSERT_CALL(AssertState(V2_ControlSm_StateId_OFF));
    ASSERT_CALL(AssertLights("___"));
}

TEST_F(V2_ControlTests3, On1_DimTo_Off)
{
    ASSERT_CALL(ReachOn1());

    DimPress();
    ASSERT_CALL(AssertState(V2_ControlSm_StateId_OFF));
    ASSERT_CALL(AssertLights("___"));
}

TEST_F(V2_ControlTests3, On1_IncTo_On2)
{
    ASSERT_CALL(ReachOn2());
}

TEST_F(V2_ControlTests3, On2_DimTo_On1)
{
    ASSERT_CALL(ReachOn2());

    DimPress();
    ASSERT_CALL(AssertState(V2_ControlSm_StateId_ON1));
    ASSERT_CALL(AssertLights("#__"));
}

TEST_F(V2_ControlTests3, On2_IncTo_On3)
{
    ASSERT_CALL(ReachOn3());
}

TEST_F(V2_ControlTests3, On3_DimTo_On2)
{
    ASSERT_CALL(ReachOn3());

    DimPress();
    ASSERT_CALL(AssertState(V2_ControlSm_StateId_ON2));
    ASSERT_CALL(AssertLights("##_"));
}

TEST_F(V2_ControlTests3, On3_ignore_inc)
{
    ASSERT_CALL(ReachOn3());

    IncPress();
    ASSERT_CALL(AssertState(V2_ControlSm_StateId_ON3));
    ASSERT_CALL(AssertLights("###"));
}

//////////////////////////

TEST_F(V2_ControlTests3, On1_OffTo_Off)
{
    ASSERT_CALL(ReachOn1());

    OffPress();
    ASSERT_CALL(AssertState(V2_ControlSm_StateId_OFF));
    ASSERT_CALL(AssertLights("___"));
}

TEST_F(V2_ControlTests3, On2_OffTo_Off)
{
    ASSERT_CALL(ReachOn2());

    OffPress();
    ASSERT_CALL(AssertState(V2_ControlSm_StateId_OFF));
    ASSERT_CALL(AssertLights("___"));
}

TEST_F(V2_ControlTests3, On3_OffTo_Off)
{
    ASSERT_CALL(ReachOn3());

    OffPress();
    ASSERT_CALL(AssertState(V2_ControlSm_StateId_OFF));
    ASSERT_CALL(AssertLights("___"));
}

///////////////////////////

TEST_F(V2_ControlTests3, OnGroup_OffTo_Off)
{
    std::vector<std::function<void()>> reach_methods = {
        [this]{ ReachOn1(); },
        [this]{ ReachOn2(); },
        [this]{ ReachOn3(); }
    };

    for (auto& reach_method : reach_methods)
    {
        ASSERT_CALL(reach_method());

        OffPress();
        ASSERT_CALL(AssertState(V2_ControlSm_StateId_OFF));
        ASSERT_CALL(AssertLights("___"));
    }
}

///////////////////////////

/**
 * This test shows how you can use a StateSmith feature to easily ensure that you
 * don't miss a single substate as a design evolves.
 */
TEST_F(V2_ControlTests3, OnGroup_OffTo_Off_ForHugeDesign)
{
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

        ASSERT_CALL(reach_method());

        OffPress();
        ASSERT_CALL(AssertState(V2_ControlSm_StateId_OFF));
        ASSERT_CALL(AssertLights("___"));
    }
}
