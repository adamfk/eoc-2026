// ✅ This test file uses helpers so that we can efficiently specify Natural Flow tests and avoid huge repetition.
// ❌ When a test fails, the GTEST output is not very helpful.
//     Try modifying the FSM (insert a state between OFF and ON1), and run theses tests:
//     GTEST_BRIEF=1 GTEST_FILTER=*Tests1.*  cmake --build . --target run_tests
// ⏩ We will improve later.

extern "C"
{
#include "V2/Control.h"
}
#include "FakeHal.hpp"
#include "gtest_helpers.hpp"
#include <gtest/gtest.h>
#include <functional>

class V2_ControlTests1 : public ::testing::Test
{
protected:
    FakeHal hal;

    V2_ControlTests1()
    {
        V2_Control_setup();
    }

    void SetUp() override
    {
        // runs after constructor.
        // you can use ASSERT_ macros here.
        AssertState(V2_ControlSm_StateId_OFF);
        AssertLights("___");
    }

    void AssertLights(std::string_view lights)
    {
        hal.AssertOutputs(lights);
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
        AssertState(V2_ControlSm_StateId_ON1);
        AssertLights("#__");
    }

    void ReachOn2()
    {
        ReachOn1();

        IncPress();
        AssertState(V2_ControlSm_StateId_ON2);
        AssertLights("##_");
    }

    void ReachOn3()
    {
        ReachOn2();

        for (size_t i = 0; i < 3 - 1; i++)
        {
            IncPress();
            AssertState(V2_ControlSm_StateId_ON2);
            AssertLights("##_");
        }

        IncPress();
        AssertState(V2_ControlSm_StateId_ON3);
        AssertLights("###");
    }
};

TEST_F(V2_ControlTests1, Off_ignores_dim__and_off)
{
    DimPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");

    OffPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

TEST_F(V2_ControlTests1, Off_IncTo_On1)
{
    ReachOn1();
}

TEST_F(V2_ControlTests1, On1_DimTo_Off)
{
    ReachOn1();

    DimPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

TEST_F(V2_ControlTests1, On1_IncTo_On2)
{
    ReachOn2();
}

TEST_F(V2_ControlTests1, On2_DimTo_On1)
{
    ReachOn2();

    DimPress();
    AssertState(V2_ControlSm_StateId_ON1);
    AssertLights("#__");
}

TEST_F(V2_ControlTests1, On2_IncTo_On3)
{
    ReachOn3();
}

TEST_F(V2_ControlTests1, On3_DimTo_On2)
{
    ReachOn3();

    DimPress();
    AssertState(V2_ControlSm_StateId_ON2);
    AssertLights("##_");
}

TEST_F(V2_ControlTests1, On3_ignore_inc)
{
    ReachOn3();

    IncPress();
    AssertState(V2_ControlSm_StateId_ON3);
    AssertLights("###");
}

///////////// Testing Hierarchical OFF behavior /////////////

TEST_F(V2_ControlTests1, On1_OffTo_Off)
{
    ReachOn1();

    OffPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

TEST_F(V2_ControlTests1, On2_OffTo_Off)
{
    ReachOn2();

    OffPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

TEST_F(V2_ControlTests1, On3_OffTo_Off)
{
    ReachOn3();

    OffPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

