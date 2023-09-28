#pragma once

#include <stdint.h>

void initTIM(TIM_TypeDef* TIMx, uint16_t dutycycle, uint32_t frequency);
void update_dutycycle(TIM_TypeDef* TIMx, uint32_t dutycycle);
void togglePWM(TIM_TypeDef* TIMx);
void switchPWM(TIM_TypeDef* TIMx, uint8_t HL);
void initPWM(GPIO_TypeDef* Port, uint8_t pin, TIM_TypeDef* TIMx);
