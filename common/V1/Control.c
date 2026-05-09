#include "Control.h"
#include "Lights.h"

// private vars
static uint8_t count;
static V1_StateId state;

static void dispatch_event(V1_EventId event);

// should only be called once at startup (unless unit testing)
void V1_Control_setup(void)
{
    count = 0;
    state = V1_StateId_OFF;
}

void V1_Control_handle_event(V1_EventId event)
{
    dispatch_event(event);
}

V1_StateId V1_Control_get_state(void)
{
    return state;
}

// example invalid cpp code from V0. Compiles fine here because we don't include into a .cpp file.
int v1_invalid_cpp_1(void)
{
    int class = 22; // 'class' is a c++ keyword
    return class;
}

// example invalid cpp code from V0. Compiles fine here because we don't include into a .cpp file.
int v1_invalid_cpp_2(void)
{
    int arr[5] = { [1] = 10, [4] = 20 }; // Valid C11, Invalid C++20
    return arr[1];
}

// This define is only present for unit testing build.
#ifdef UNIT_TESTING
void UNIT_TESTING_V1_Control_set_state(V1_StateId state_arg)
{
    state = state_arg;
}
void UNIT_TESTING_V1_Control_set_count(uint8_t count_arg)
{
    count = count_arg;
}
#endif

//----------------------------------------------------------------------
// !!!!!!!!!!!!!!!!!! STATE MACHINE HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//----------------------------------------------------------------------

/*
    @startuml V1_Control
    state OFF
    state ON1
    state ON2
    state ON3

    [*] -> OFF
    OFF --> ON1: INC
    ON1 -up-> OFF : DIM
    ON1 -down-> ON2 : INC
    ON2 -up-> ON1 : DIM
    ON2 -down-> ON3 : INC [count >= 3]
    ON3 -up-> ON2 : DIM

    OFF: enter / Lights_off();
    ON1: enter / Lights_set(1,0,0);
    ON2: enter / Lights_set(1,1,0);
    ON2: enter / count = 0;
    ON2: 1. INC / count++;
    ON3: enter / Lights_set(1,1,1);
    @enduml
*/


// mostly AI GENERATED CODE BELOW !!!!!!!!!!!!!!!!!!!!!!!!!
// same as V0 code.

static void enter_OFF(void)
{
    state = V1_StateId_OFF;
    Lights_off();
}

static void enter_ON1(void)
{
    state = V1_StateId_ON1;
    Lights_set(1, 0, 0);
}

static void enter_ON2(void)
{
    state = V1_StateId_ON2;
    Lights_set(1, 1, 0);
    count = 0;
}

static void enter_ON3(void)
{
    state = V1_StateId_ON3;
    Lights_set(1, 1, 1);
}

static void dispatch_event(V1_EventId event)
{
    switch (state)
    {
        case V1_StateId_OFF:
            if (event == V1_EventId_INC) { enter_ON1(); }
            break;

        case V1_StateId_ON1:
            if (event == V1_EventId_DIM) { enter_OFF(); }
            else if (event == V1_EventId_INC) { enter_ON2(); }
            break;

        case V1_StateId_ON2:
            if (event == V1_EventId_DIM)      { enter_ON1(); }
            else if (event == V1_EventId_INC)
            {
                count++;
                if (count >= 3) { enter_ON3(); }
            }
            break;

        case V1_StateId_ON3:
            if (event == V1_EventId_DIM) { enter_ON2(); }
            break;
    }
}
