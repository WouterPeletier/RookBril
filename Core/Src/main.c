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
	init_gpio(GPIOA, GPIO_0, GPIO_MODER_ALT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);
	gpio_toggle(GPIOA, GPIO_0);
	initTIMIRS(50, 38000);
	IRInit(0b00110, Send);
	for(volatile uint32_t j = 0; j<400000; j++) {
	}
	IRSend(0b011010);
	while(1){
	}
}

void Systick_Handler() {

}


