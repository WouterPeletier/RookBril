#pragma once

#include <stdint.h>

void init_pwm(TIM_TypeDef* TIMx, uint16_t dutycycle, uint32_t frequency, GPIO_TypeDef* Port, uint8_t pin);
void update_dutycycle(TIM_TypeDef* TIMx, uint32_t dutycycle);
void togglePWM(TIM_TypeDef* TIMx);
void switchPWM(TIM_TypeDef* TIMx, uint8_t HL);
