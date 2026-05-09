#include "Control.h"
#include "Lights.h"

// private enum
typedef enum V0_StateId
{
    V0_StateId_OFF = 0,
    V0_StateId_ON1 = 1,
    V0_StateId_ON2 = 2,
    V0_StateId_ON3 = 3
} V0_StateId;

// private vars
static uint8_t count = 0;
static V0_StateId state = V0_StateId_OFF;

// function prototypes
static void dispatch_event(V0_EventId event);

void V0_Control_handle_event(V0_EventId event)
{
    dispatch_event(event);
}

// Some example C code that is invalid C++ code.
// Causes a problem when this .c file is included into .cpp file for testing.
// Other approaches (V1/V2) overcome this issue.
#if USE_INVALID_C_CODE_FOR_CPP
int invalid_cpp_1(void)
{
    int class = 22; // 'class' is a c++ keyword
    return class;
}

int invalid_cpp_2(void)
{
    int arr[5] = { [1] = 10, [4] = 20 }; // Valid C11, Invalid C++20
    return arr[1];
}
#endif

//----------------------------------------------------------------------
// !!!!!!!!!!!!!!!!!! STATE MACHINE HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//----------------------------------------------------------------------

/*
    @startuml V0_Control
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


// Mostly AI GENERATED CODE BELOW (from plantuml).
// Just need a small touch up.

static void enter_OFF(void)
{
    state = V0_StateId_OFF;
    Lights_off();
}

static void enter_ON1(void)
{
    state = V0_StateId_ON1;
    Lights_set(1, 0, 0);
}

static void enter_ON2(void)
{
    state = V0_StateId_ON2;
    Lights_set(1, 1, 0);
    count = 0;
}

static void enter_ON3(void)
{
    state = V0_StateId_ON3;
    Lights_set(1, 1, 1);
}

static void dispatch_event(V0_EventId event)
{
    switch (state)
    {
        case V0_StateId_OFF:
            if (event == V0_EventId_INC) { enter_ON1(); }
            break;

        case V0_StateId_ON1:
            if (event == V0_EventId_DIM) { enter_OFF(); }
            else if (event == V0_EventId_INC) { enter_ON2(); }
            break;

        case V0_StateId_ON2:
            if (event == V0_EventId_DIM)      { enter_ON1(); }
            else if (event == V0_EventId_INC)
            {
                count++;
                if (count >= 3) { enter_ON3(); }
            }
            break;

        case V0_StateId_ON3:
            if (event == V0_EventId_DIM) { enter_ON2(); }
            break;
    }
}
