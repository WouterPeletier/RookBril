
/*
 * IR.c
 *
 *  Created on: Sep 28, 2023
 *      Author: Mathijs, Kevin
 *
 *  Using the following peripherals:
 *  	TIM3, TIM2 for sending
 */

#include "pwm.h"
#include "IR.h"
extern bool sendFlag;

#define lowDuration 600
uint8_t IRData[14];
uint32_t i = 0;
uint8_t periodTag = 0;

void IRInit(uint8_t address, IRMode mode, GPIO_TypeDef* Port, uint8_t pin) {
	if(mode == Send) {
		IRData[0] = 1; //start bit
		IRData[1] = 1; //Field bit
		IRData[2] = 1; //Control bit
		int l = 0;
		for(int j=4; j>=0; j--) {
			IRData[7-j] = (address >> j) & 1;
		}

		initTIM(TIM3, 50, 38000); //TIM3 38kHz
		initPWM(Port, pin, TIM3); //put TIM3 output to Port, pin
		int tim2f = (1000000/600);
		initTIM(TIM2, 50, tim2f); //TIM2 600us freq
		switchPWM(TIM2, 1); //turn on TIM2
		NVIC_EnableIRQ(TIM2_IRQn); //enable interrupts for TIM2
	} else if(mode == Receive) {
		initTIM(TIM4, 50, (1/lowDuration)*1000000); // TIM4 600us freq
		set_interrupt(Port, pin, 3);
	}
}

void IRSend(uint8_t data) {
	if(data >= 64) {
		return;
	}
	sendFlag = true;
	i = 0;
	for(int j = 5; j>=0; j--) {
		IRData[13-j] = (data >> j) & 1;
	}
	TIM2->DIER |= TIM_DIER_UIE; //enable TIM2 interrupt
}

void IRSendBit(uint8_t data) {
	if(data == 1) {
		if(periodTag == 0) {
			switchPWM(TIM3, 1); //turn on tim3 pwm output
			periodTag++;
		} else {
			switchPWM(TIM3, 0); //turn off tim 3 pwm output
			periodTag = 0;
			i++;
		}
	} else {
		if(periodTag == 0) {
			switchPWM(TIM3, 0); //turn on tim3 pwm output
			periodTag++;
		} else {
			switchPWM(TIM3, 1); //turn off tim 3 pwm output
			periodTag = 0;
			i++;
		}
	}
}

void TIM2_IRQHandler(void) {
	TIM2->SR &= !TIM_SR_UIF;
	IRSendBit(IRData[i]);
	if(i>13) {
		i = 0;
		TIM2->DIER &= ~TIM_DIER_UIE;
	}
}

/*Dingen die moeten worden gecontrolleerd:
 * TIM4 voor het ontvangen?
 * TIM2
 *
 */
