#include "Lights.h"
#include "Hal.h"

#include <stdio.h>
#include <assert.h>

void Lights_off(void)
{
    for (uint8_t i = 0; i < Hal_MAX_OUTPUTS; i++)
    {
        Hal_set_output(i, false);
    }
}

void Lights_set(uint8_t led_0_state, uint8_t led_1_state, uint8_t led_2_state)
{
    Hal_set_output(0, led_0_state != 0);
    Hal_set_output(1, led_1_state != 0);
    Hal_set_output(2, led_2_state != 0);
}

// range is inclusive
void Lights_set_range(uint8_t from, uint8_t to)
{
    Lights_off();

    // protect against out of bounds
    if (to >= Hal_MAX_OUTPUTS)
    {
        to = Hal_MAX_OUTPUTS - 1;
    }

    static_assert(Hal_MAX_OUTPUTS < UINT8_MAX, "for loop index integer type must be larger");
    for (uint8_t i = from; i <= to; i++)
    {
        Hal_set_output(i, true);
    }
}
