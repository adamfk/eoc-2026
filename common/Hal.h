#pragma once
#include <stdint.h>
#include <stdbool.h>

#define Hal_MAX_OUTPUTS 3

void Hal_set_output(uint8_t index, bool state);
