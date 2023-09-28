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

typedef enum {Send, Receive}IRMode;

typedef struct _IRPacket{
	uint8_t IRAddress;
	uint8_t IRData;
} IRPacket;

#endif /* INC_IR_H_ */
