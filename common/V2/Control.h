#pragma once
#include "V2_ControlSm.h"

// should only be called once at startup (unless unit testing)
void V2_Control_setup(void);

// Often helpful to allow querying a FSM running on a real target.
V2_ControlSm_StateId V2_Control_get_state(void);

// Below function provide a more intentional API to users of the control module.
// V0 and V1 "leak" FSM events outside of control module. Often fine, but
// sometimes you have special events that Control code sends to its own FSM.
// You often don't want external API users to use or be aware of these special events.
// Not required, just an example.

void V2_Control_inc_press(void);
void V2_Control_dim_press(void);
void V2_Control_off_press(void);
