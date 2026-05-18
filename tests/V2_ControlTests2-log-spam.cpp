// This is a variant of V2_ControlTests2_LogSpam.cpp.
// ✅ This test file uses helpers so that we can efficiently specify Natural Flow tests and avoid huge repetition.
// ✅ Outputs stack trace like information.
// ❌ It spams output even when there's no failure. This could probably be overcome using test framework specific functionality.

// Run just these tests with: GTEST_FILTER="*Spam.*" cmake --build . --target run_tests

/*
Example output when transition logic between ON1 and ON2 states changes and tests are broken:

    [ RUN      ] V2_ControlTests2_LogSpam.On3_OffTo_Off
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:34    |SetUp (enter)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:55    |    AssertState (enter)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:65    |    AssertState (exit)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:48    |    AssertLights (enter)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:50    |    AssertLights (exit)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:43    |SetUp (exit)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:114   |ReachOn3 (enter)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:98    |    ReachOn2 (enter)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:85    |        ReachOn1 (enter)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:55    |            AssertState (enter)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:65    |            AssertState (exit)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:48    |            AssertLights (enter)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:50    |            AssertLights (exit)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:93    |        ReachOn1 (exit)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:55    |        AssertState (enter)
    /home/afk/code/eoc-2026/tests/V2_ControlTests2-log-spam.cpp:61: Failure
    Failed
    Expected state `ON2`, but was in state `ON1`.

You can scan the output and see call stack was ReachOn3 -> ReachOn2 -> AssertState
*/

extern "C"
{
#include "V2/Control.h"
}
#include "FakeHal.hpp"
#include "gtest_helpers.hpp"
#include <gtest/gtest.h>
#include <functional>

// used for printing stack trace like info
static int stack_depth = 0;
const int INDENT_SIZE = 4;
#define LOG_SPAM_ENTER() printf("%s:%-5u |%*s%s (enter)\n", __FILE__, __LINE__, stack_depth * INDENT_SIZE, "", __func__); stack_depth++
#define LOG_SPAM_EXIT()  stack_depth--; printf("%s:%-5u |%*s%s (exit)\n", __FILE__, __LINE__, stack_depth * INDENT_SIZE, "", __func__)

class V2_ControlTests2_LogSpam : public ::testing::Test
{
protected:
    FakeHal hal;

    V2_ControlTests2_LogSpam()
    {
        stack_depth = 0;
        V2_Control_setup();
    }

    void SetUp() override
    {
        LOG_SPAM_ENTER();

        // runs after constructor.
        // you can use ASSERT_ macros here.
        AssertState(V2_ControlSm_StateId_OFF);
        RETURN_IF_FAILURE();
        AssertLights("___");
        RETURN_IF_FAILURE();

        LOG_SPAM_EXIT();
    }

    void AssertLights(std::string_view lights)
    {
        LOG_SPAM_ENTER();
        hal.AssertOutputs(lights);
        LOG_SPAM_EXIT();
    }

    void AssertState(V2_ControlSm_StateId expected_state)
    {
        LOG_SPAM_ENTER();

        const V2_ControlSm_StateId actual_state = V2_Control_get_state();

        if (expected_state != actual_state)
        {
            FAIL() << "Expected state `" << V2_ControlSm_state_id_to_string(expected_state)
                   << "`, but was in state `" << V2_ControlSm_state_id_to_string(actual_state) << "`.";
        }

        LOG_SPAM_EXIT();
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
        LOG_SPAM_ENTER();

        IncPress();
        AssertState(V2_ControlSm_StateId_ON1);
        RETURN_IF_FAILURE();
        AssertLights("#__");
        RETURN_IF_FAILURE();

        LOG_SPAM_EXIT();
    }

    void ReachOn2()
    {
        LOG_SPAM_ENTER();

        ReachOn1();
        RETURN_IF_FAILURE();

        IncPress();
        AssertState(V2_ControlSm_StateId_ON2);
        RETURN_IF_FAILURE();
        AssertLights("##_");
        RETURN_IF_FAILURE();

        LOG_SPAM_EXIT();
    }

    void ReachOn3()
    {
        LOG_SPAM_ENTER();

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

        LOG_SPAM_EXIT();
    }
};

//////////////////////////////////// START OF TESTS ////////////////////////////////////

TEST_F(V2_ControlTests2_LogSpam, Off_IncTo_On1)
{
    LOG_SPAM_ENTER(); // helps navigate directly to test body from failure logs. Optional.
    ReachOn1();
}

TEST_F(V2_ControlTests2_LogSpam, Off_ignores_dim__and_off)
{
    DimPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
    RETURN_IF_FAILURE();

    OffPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

TEST_F(V2_ControlTests2_LogSpam, On1_DimTo_Off)
{
    ReachOn1();
    RETURN_IF_FAILURE();

    DimPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

TEST_F(V2_ControlTests2_LogSpam, On1_IncTo_On2)
{
    ReachOn2();
}

TEST_F(V2_ControlTests2_LogSpam, On2_DimTo_On1)
{
    ReachOn2();
    RETURN_IF_FAILURE();

    DimPress();
    AssertState(V2_ControlSm_StateId_ON1);
    AssertLights("#__");
}

TEST_F(V2_ControlTests2_LogSpam, On2_IncTo_On3)
{
    ReachOn3();
    RETURN_IF_FAILURE();
}

TEST_F(V2_ControlTests2_LogSpam, On3_DimTo_On2)
{
    ReachOn3();
    RETURN_IF_FAILURE();

    DimPress();
    AssertState(V2_ControlSm_StateId_ON2);
    AssertLights("##_");
}

TEST_F(V2_ControlTests2_LogSpam, On3_ignore_inc)
{
    ReachOn3();
    RETURN_IF_FAILURE();

    IncPress();
    AssertState(V2_ControlSm_StateId_ON3);
    AssertLights("###");
}

//////////////////////////

TEST_F(V2_ControlTests2_LogSpam, On1_OffTo_Off)
{
    ReachOn1();
    RETURN_IF_FAILURE();

    OffPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

TEST_F(V2_ControlTests2_LogSpam, On2_OffTo_Off)
{
    ReachOn2();
    RETURN_IF_FAILURE();

    OffPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}

TEST_F(V2_ControlTests2_LogSpam, On3_OffTo_Off)
{
    ReachOn3();
    RETURN_IF_FAILURE();

    OffPress();
    AssertState(V2_ControlSm_StateId_OFF);
    AssertLights("___");
}
