#pragma once

typedef enum V0_EventId
{
    V0_EventId_DIM = 0,
    V0_EventId_INC = 1,
} V0_EventId;

void V0_Control_handle_event(V0_EventId);
