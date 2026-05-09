#include "Hal.h"
#include <stdio.h>

#define COUNTOF(a) (sizeof(a) / sizeof(a[0]))

static bool outputs[Hal_MAX_OUTPUTS];

void SimHal_print_outputs(void)
{
    printf("SIM HAL OUTPUTS: ");
    for (size_t i = 0; i < COUNTOF(outputs); i++)
    {
        const bool output_state = outputs[i];
        const char c = output_state ? '#' : '_';
        printf("%c ", c);
    }
    printf("\n");
}

void Hal_set_output(uint8_t index, bool state)
{
    if (index > COUNTOF(outputs))
    {
        // could assert or print here too
        return;
    }

    outputs[index] = state;
}
