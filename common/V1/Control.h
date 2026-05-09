#pragma once
#include <stdint.h>

typedef enum V1_EventId
{
    V1_EventId_DIM = 0,
    V1_EventId_INC = 1,
} V1_EventId;

// Public compared to V0.
typedef enum V1_StateId
{
    V1_StateId_OFF = 0,
    V1_StateId_ON1 = 1,
    V1_StateId_ON2 = 2,
    V1_StateId_ON3 = 3
} V1_StateId;

// should only be called once at startup (unless unit testing)
void V1_Control_setup(void);
void V1_Control_handle_event(V1_EventId);

// Often helpful to allow querying a FSM running on a real target.
V1_StateId V1_Control_get_state(void);

// Required for this style of 'Hand of God' testing.
// Even if a sneaky dev wanted access to these functions for production code, they couldn't just define `UNIT_TESTING` before including this file.
// The linker would fail to find the functions.
#ifdef UNIT_TESTING
void UNIT_TESTING_V1_Control_set_state(V1_StateId state);
void UNIT_TESTING_V1_Control_set_count(uint8_t count);
#endif
