
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

void IRInit(uint8_t address, IRMode mode) {
	if(mode == Send) {
		IRData[0] = 1; //start bit
		IRData[1] = 1; //Field bit
		IRData[2] = 1; //Control bit
		int l = 0;
		for(int j=4; j>=0; j--) {
			IRData[7-j] = (address >> j) & 1;
		}
		int tim2f = (1000000/lowDuration);
		initTIMIRGeneric(50, tim2f); //Generic 600us timer
		initTIMIRS(50, 38000); //Send timer op 38kHz
		NVIC_EnableIRQ(TIM2_IRQn); //enable interrupts for TIM2
	} else if(mode == Receive) {
		initTIMIRR(50, (1/lowDuration)*1000000); // TIM4 600us freq
		set_interrupt(GPIOA, GPIO_0, 3);
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
	switchPWM(TIM2, 1);
	TIM2->DIER |= TIM_DIER_UIE; //enable TIM2 interrupt
}

void IRSendBit(uint8_t data) {
	if(data == 1) {
		if(periodTag == 0) {
			switchPWM(TIM5, 1); //turn on tim3 pwm output
			periodTag++;
		} else {
			switchPWM(TIM5, 0); //turn off tim 3 pwm output
			TIM5->CNT = 0;
			periodTag = 0;
			i++;
		}
	} else {
		if(periodTag == 0) {
			switchPWM(TIM5, 0); //turn off tim3 pwm output
			TIM5->CNT = 0;
			periodTag++;
		} else {
			switchPWM(TIM5, 1); //turn on tim 3 pwm output

			periodTag = 0;
			i++;
		}
	}
//	gpio_write(GPIOA, GPIO_1, 0);
}

void TIM2_IRQHandler(void) {
	TIM2->SR &= !TIM_SR_UIF;
	IRSendBit(IRData[i]);
	if(i>13 && (periodTag == 1)) {
		i = 0;
		TIM2->DIER &= ~TIM_DIER_UIE;
		switchPWM(TIM5, 0);
		TIM5->CNT = 0;
	}
}

/*Dingen die moeten worden gecontrolleerd:
 * TIM4 voor het ontvangen?
 * TIM2
 *
 */
