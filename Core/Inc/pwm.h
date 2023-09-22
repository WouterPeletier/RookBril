#pragma once

#include <stdint.h>

void init_pwm(uint16_t dutycycle, uint32_t frequency);
void update_dutycycle(uint32_t dutycycle);
