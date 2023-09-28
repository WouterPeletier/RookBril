/*
 * IR.h
 *
 *  Created on: Sep 28, 2023
 *      Author: Mathijs
 */

#ifndef INC_IR_H_
#define INC_IR_H_

#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include "gpio.h"

//Verander dit naar de correct pin
#define IRReceivePort GPIO_D;
#define IRReceivePin GPIO_3;

#define IRTransmitPort GPIO_D;
#define IRTransmitPin GPIO_13;

typedef enum {Send, Receive}IRMode;

typedef struct _IRPacket{
	uint8_t IRAddress;
	uint8_t IRData;
} IRPacket;

void IRInit(uint8_t address, IRMode mode);

#endif /* INC_IR_H_ */
