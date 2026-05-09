#include "Control.h"
#include "V2_ControlSm.h"
#include "Lights.h"

// private vars
static uint8_t count;
static V2_ControlSm control_sm;

// should only be called once at startup (unless unit testing)
void V2_Control_setup(void)
{
    V2_ControlSm_ctor(&control_sm);
    V2_ControlSm_start(&control_sm);
}

void V2_Control_inc_press(void)
{
    V2_ControlSm_dispatch_event(&control_sm, V2_ControlSm_EventId_INC);
}

void V2_Control_dim_press(void)
{
    V2_ControlSm_dispatch_event(&control_sm, V2_ControlSm_EventId_DIM);
}

void V2_Control_off_press(void)
{
    V2_ControlSm_dispatch_event(&control_sm, V2_ControlSm_EventId_OFF);
}

V2_ControlSm_StateId V2_Control_get_state(void)
{
    return control_sm.state_id;
}

//------------------------------------------------------------------------------
// !!!!!!!!!!!!!!!!!! STATE MACHINE INCLUDE HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//------------------------------------------------------------------------------
// This should typically happen at bottom of file.
// See https://github.com/StateSmith/StateSmith-examples/blob/main/c-include-sm-basic-2-plantuml-tutorial/
#include "V2_ControlSm.inc"
