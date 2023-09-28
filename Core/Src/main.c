#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include <stdbool.h>

#include "platform.h"
#include "gpio.h"
#include "pwm.h"
#include "interrupts.h"
#include "IR.h"

#define Address = 1; //Address tussen 0 en 32

IRMode IRSendReceive = Send;
IRPacket * IRMsg = {0};

uint32_t bitCycles = 4800; //De tijd van de bit in clock cycles = 600us/(1/F_CPU (8000000))
uint32_t sendIntervalms = 1000; //De tijd in ms hoelang het duurt tot de volgende IR send command

int main(void)
{
	IRMsg->IRAddress = 1;

	init_platform();
	init_pwm(TIM3, 25, 1000, GPIOA, GPIO_6);
	togglePWM(TIM3);
	update_dutycycle(TIM3, 50);
	while(1){}
	//printf("Done\n");
	while(1)
	{
		update_dutycycle(TIM3, 1);
		delay_ms(2000);
		for (int i = 1; i<25; i++)
		{
			delay_ms(100);
			update_dutycycle(TIM3, i);
		}
		
 		delay_ms(2000);
	}

}

void InitIR(uint8_t address, IRMode mode) {
	init_pwm(TIM3, 50, 38000, GPIOA, GPIO_6);
}

