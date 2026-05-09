#pragma once
#include <stdint.h>

void Lights_off(void);

void Lights_set(uint8_t led_0_state, uint8_t led_1_state, uint8_t led_2_state);

// range is inclusive
void Lights_set_range(uint8_t from, uint8_t to);
