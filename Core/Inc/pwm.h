#pragma once

#include <stdint.h>
#include <stm32f4xx.h>
/*
 * Timer Assignments:
 * TIM1:
 * TIM2: IR Send 600us interval met interrupt
 * TIM3: PDLC Met GPIO A6 A7
 * TIM4: IR Receiver zonder GPIO gemeten ~600us
 * TIM5: IR Send 38kHz met GPIO
 * TIM9:
 * TIM10:
 * TIM11:
 * TIM12:
 *
 */

void initTimPDLC2(uint16_t dutycycle, uint32_t frequency);
void initTIMPDLC(uint16_t dutycycle, uint32_t frequency);
void initTIMIRR(uint16_t dutycycle, uint32_t frequency); //Receive
void initTIMIRS(uint16_t dutycycle, uint32_t frequency); //Send timer
void initTIMIRGeneric(uint16_t dutycycle, uint32_t frequency); //600 us timer
void update_dutycycle(TIM_TypeDef* TIMx, uint32_t dutycycle);
void togglePWM(TIM_TypeDef* TIMx);
void switchPWM(TIM_TypeDef* TIMx, uint8_t HL);