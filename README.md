Repo for 2026 Embedded Online Conference talk [Test Your State Machine Monstrosities](https://embeddedonlineconference.com/session/Test_Your_State_Machine_Monstrosities)

# Focus
This repo is primarily focused on state machine testing while trying to keep things reasonably simple and easy to follow.

We could improve cmake setup significantly, but that's a complete topic on its own.

# Setup
I'm using Windows with WSL2, but this will also work for Linux and mac.

We need to install:
* gcc: c and c++ compiler
* gdb: debugger
* lcov: visual code coverage tool
* cmake: build tool

## WSL2/Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install -y build-essential gdb lcov cmake
```

## mac (using Homebrew)
```bash
brew install gcc gdb lcov cmake
```

# Build
Standard cmake workflow.

```bash
$ mkdir build
$ cd build
build$ cmake ..
build$ cmake --build .
```

# Run Commands
### Run Simulation
```bash
build$ cmake --build . --target run_sim
```

### Run Tests
```bash
build$ cmake --build . --target run_tests
```
See also test tips section below

### Run Tests With Coverage
```bash
build$ cmake --build . --target coverage
```
Test report is generated to `build/tests/coverage_report/index.html`

### GTEST Environment Flags
You can customize google test behavior with various flags.

One way to do this is by using ["temporary environment flags"](https://stackoverflow.com/a/21360477) like this:
```bash
build$ GTEST_BRIEF=1 cmake --build . --target run_tests
```

Some helpful flags:
* `GTEST_BRIEF=1`: simplifies output to only show failed and summary.
* `GTEST_FILTER=LightTests.*`: run matching tests. supports wild cards.
* `GTEST_SHUFFLE=1`: randomizes the order in which tests are run.
* `GTEST_REPEAT=10`: repeats tests. Usually want to use with `GTEST_BREAK_ON_FAILURE=1` otherwise failures can be hard to spot in a lot of output.

You can set multiple flags like:
```bash
build$ GTEST_BRIEF=1 GTEST_FILTER=LightTests.* GTEST_REPEAT=10 GTEST_SHUFFLE=1 cmake --build . --target run_tests
```

You can find more [flags here](https://google.github.io/googletest/advanced.html). Note that passing flag `--gtest_shuffle` is equivalent to setting environment flag `GTEST_SHUFFLE=1`.


# Why Improve Error Messages?
It's all about scale. Small problem for small designs. Big problem for big designs.

## RETURN_IF_FAILURE
Why do we do use `RETURN_IF_FAILURE()` so much? That's just one approach (see also `tests/V2_ControlTests3.cpp`). It's all about outputting specific and focused failure messages (rather than a ton of noise).

This is particularly helpful when you have a failure in your CI/CD pipeline.

Compare the output of these two failure messages.

### ✅ With `RETURN_IF_FAILURE()` or `ASSERT_CALL()`
Simple, focused and direct.
```
[ RUN      ] ControlTestsV2.On3_DimTo_On2
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:42: Failure
Failed
Expected state `ON1`, but was in state `PRE`.
Google Test trace:
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:36: AssertState(expected_state:'3')
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:59: ReachOn1( )
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:68: ReachOn2( )
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:78: ReachOn3( )

[  FAILED  ] ControlTestsV2.On3_DimTo_On2 (0 ms)
```

## Without `RETURN_IF_FAILURE()`
A lot of repeated noise. Developers often waste time trying to figure out what to focus on.
```
[ RUN      ] ControlTestsV2.On3_DimTo_On2
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:42: Failure
Failed
Expected state `ON1`, but was in state `PRE`.
Google Test trace:
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:36: AssertState(expected_state:'3')
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:59: ReachOn1( )
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:68: ReachOn2( )
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:78: ReachOn3( )

/home/afk/code/eoc-2026/ex1/tests/FakeHal.hpp:65: Failure
Expected equality of these values:
  string
    Which is: "#__"
  GetOutputsString()
    Which is: "___"
Google Test trace:
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:30: AssertLights(lights:'#__')
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:59: ReachOn1( )
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:68: ReachOn2( )
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:78: ReachOn3( )

/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:42: Failure
Failed
Expected state `ON2`, but was in state `ON1`.
Google Test trace:
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:36: AssertState(expected_state:'4')
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:68: ReachOn2( )
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:78: ReachOn3( )

/home/afk/code/eoc-2026/ex1/tests/FakeHal.hpp:65: Failure
Expected equality of these values:
  string
    Which is: "##_"
  GetOutputsString()
    Which is: "#__"
Google Test trace:
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:30: AssertLights(lights:'##_')
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:68: ReachOn2( )
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:78: ReachOn3( )

/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:42: Failure
Failed
Expected state `ON3`, but was in state `ON2`.
Google Test trace:
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:36: AssertState(expected_state:'5')
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:78: ReachOn3( )

/home/afk/code/eoc-2026/ex1/tests/FakeHal.hpp:65: Failure
Expected equality of these values:
  string
    Which is: "###"
  GetOutputsString()
    Which is: "##_"
Google Test trace:
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:30: AssertLights(lights:'###')
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:78: ReachOn3( )

/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:42: Failure
Failed
Expected state `ON2`, but was in state `ON1`.
Google Test trace:
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:36: AssertState(expected_state:'4')

/home/afk/code/eoc-2026/ex1/tests/FakeHal.hpp:65: Failure
Expected equality of these values:
  string
    Which is: "##_"
  GetOutputsString()
    Which is: "#__"
Google Test trace:
/home/afk/code/eoc-2026/ex1/tests/ControlTestsV2.cpp:30: AssertLights(lights:'##_')

[  FAILED  ] ControlTestsV2.On3_DimTo_On2 (0 ms)
```
