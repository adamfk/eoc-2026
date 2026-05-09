// Copyright Adam Fraser-Kruck. License: MIT.
#pragma once

#include <gtest/gtest.h>

// Prefer RETURN_IF_FAILURE() or TRACE_CALL() in helper methods rather than ASSERT_NO_FATAL_FAILURE() as that often ends up repeating a ton of trace information.

// simply returns if there was a fatal failure from an ASSERT_ or a non-fatal failure from EXPECT_.
#define RETURN_IF_FAILURE()                \
    do                                     \
    {                                      \
        if (::testing::Test::HasFailure()) \
        {                                  \
            return;                        \
        }                                  \
    } while (false)

#define TRACE_CALL(code)                                                                    \
    do                                                                                      \
    {                                                                                       \
        SCOPED_TRACE(::testing::Message("Call from `") << __func__ << "` to `" #code "`."); \
        code;                                                                               \
    } while (false)

// Provides calling site information in output stack traces.
#define ASSERT_CALL(code)                                                                   \
    /* detect prior failure to help identify true failure location. */                      \
    if (::testing::Test::HasFailure())                                                      \
    {                                                                                       \
        SCOPED_TRACE("NOTE! Failure before this call.");                                    \
        FAIL();                                                                             \
        return;                                                                             \
    }                                                                                       \
    do                                                                                      \
    {                                                                                       \
        SCOPED_TRACE(::testing::Message("Call from `") << __func__ << "` to `" #code "`."); \
        code;                                                                               \
        if (::testing::Test::HasFailure())                                                  \
        {                                                                                   \
            return;                                                                         \
        }                                                                                   \
    } while (false)

#define SCOPED_TRACE_FUNC_ARGS_0()                \
    SCOPED_TRACE(::testing::Message() << __func__ \
                                      << "( )")

#define SCOPED_TRACE_FUNC_ARGS_1(arg1)            \
    SCOPED_TRACE(::testing::Message() << __func__ \
                                      << "(" << #arg1 << ":'" << arg1 << "')")

#define SCOPED_TRACE_FUNC_ARGS_2(arg1, arg2)                            \
    SCOPED_TRACE(::testing::Message() << __func__                       \
                                      << "("                            \
                                      << #arg1 << ":'" << arg1 << "', " \
                                      << #arg2 << ":'" << arg2 << "' "  \
                                      << ")")

#define SCOPED_TRACE_FUNC_ARGS_3(arg1, arg2, arg3)                      \
    SCOPED_TRACE(::testing::Message() << __func__                       \
                                      << "("                            \
                                      << #arg1 << ":'" << arg1 << "', " \
                                      << #arg2 << ":'" << arg2 << "', " \
                                      << #arg3 << ":'" << arg3 << "' "  \
                                      << ")")

#define SCOPED_TRACE_FUNC_ARGS_4(arg1, arg2, arg3, arg4)                \
    SCOPED_TRACE(::testing::Message() << __func__                       \
                                      << "("                            \
                                      << #arg1 << ":'" << arg1 << "', " \
                                      << #arg2 << ":'" << arg2 << "', " \
                                      << #arg3 << ":'" << arg3 << "', " \
                                      << #arg4 << ":'" << arg4 << "' "  \
                                      << ")")

#define SCOPED_TRACE_FUNC_ARGS_5(arg1, arg2, arg3, arg4, arg5)          \
    SCOPED_TRACE(::testing::Message() << __func__                       \
                                      << "("                            \
                                      << #arg1 << ":'" << arg1 << "', " \
                                      << #arg2 << ":'" << arg2 << "', " \
                                      << #arg3 << ":'" << arg3 << "', " \
                                      << #arg4 << ":'" << arg4 << "', " \
                                      << #arg5 << ":'" << arg5 << "' "  \
                                      << ")")

// below is some macro magic that figures out which macro depending on the number of
// arguments (max of 5), so you can just do `SCOPED_TRACE_FUNC(expected_state, actual_state)`.

#define GET_MACRO_FOR_SCOPED_TRACE_FUNC_ARGS(_0, _1, _2, _3, _4, _5, NAME, ...) NAME

#define SCOPED_TRACE_FUNC(...)                                     \
    GET_MACRO_FOR_SCOPED_TRACE_FUNC_ARGS(_0, ##__VA_ARGS__,        \
                                         SCOPED_TRACE_FUNC_ARGS_5, \
                                         SCOPED_TRACE_FUNC_ARGS_4, \
                                         SCOPED_TRACE_FUNC_ARGS_3, \
                                         SCOPED_TRACE_FUNC_ARGS_2, \
                                         SCOPED_TRACE_FUNC_ARGS_1, \
                                         SCOPED_TRACE_FUNC_ARGS_0)(__VA_ARGS__)

// Try uncommenting below lines with some V2 test failures
// #define SCOPED_TRACE(arg)
// #define RETURN_IF_FAILURE()
