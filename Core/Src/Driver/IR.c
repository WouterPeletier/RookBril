/*
 * IR.c
 *
 *  Created on: Sep 28, 2023
 *      Author: Mathijs
 */

#include "pwm.h"
#include "IR.h"

void IRInit(uint8_t address, IRMode mode) {
	if(mode == Send) {
		initTIM(TIM2, 50, 38000);
		initPWM(GPIOD, GPIO_13);
	} else if(mode == Receive) {
		set_interrupt(IRTransmitPort, IRTransmitPin);
	}
}
