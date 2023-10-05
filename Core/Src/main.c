#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include <stdbool.h>

#include "platform.h"
#include "gpio.h"
#include "pwm.h"
#include "interrupts.h"
#include "IR.h"

#define Receiving
#define Address = 1; //Address tussen 0 en 32

IRMode IRSendReceive = Send;
IRPacket * IRMsg = {0};
uint32_t bitCycles = 4800; //De tijd van de bit in clock cycles = 600us/(1/F_CPU (8000000))
uint32_t sendIntervalms = 1000; //De tijd in ms hoelang het duurt tot de volgende IR send command
bool sendFlag = false;

int main(void) {
	IRMsg->IRAddress = 1;

	init_platform();
	IRInit(0b00110, Send, GPIOA, GPIO_6);
	IRSend(0b011010);
	while(1){}
	//printf("Done\n");
	while(1)
	{
		sendIR(&IRMsg);
	}
}

void Systick_Handler() {

}


