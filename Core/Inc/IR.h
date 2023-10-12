/*
 * IR.h
 *
 *  Created on: Sep 28, 2023
 *      Author: Mathijs
 */
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include <stdbool.h>
#include "gpio.h"

typedef enum {Send, Receive}IRMode;

typedef struct _IRPacket{
	uint8_t IRAddress;
	uint8_t IRData;
} IRPacket;

void IRInit(uint8_t address, IRMode mode);
void IRSend(uint8_t data);
