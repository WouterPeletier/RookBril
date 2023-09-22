#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>

#include "platform.h"
#include "gpio.h"
#include "pwm.h"
#include "interrupts.h"

#ifdef SEMIHOSTING
/* Semihosting */
extern void initialise_monitor_handles(void);
#endif

int main(void)
{
	#ifdef SEMIHOSTING
	initialise_monitor_handles();	/* init semihosting */
	#endif

	init_platform();
	init_pwm(25, 1000);
	set_interrupt(GPIOA, GPIO_4, INT_FALLING_EDGE);
	
	/* Adafruit implementation of ac output */
	init_gpio(GPIOD, GPIO_15, GPIO_MODER_OUTPUT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);
	init_gpio(GPIOD, GPIO_13, GPIO_MODER_OUTPUT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);
	
	/* CLKOUT */
	init_gpio(GPIOC, GPIO_9, GPIO_MODER_ALT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_VERYHIGH);	//=SYSCLK/4

	gpio_write(GPIOD, GPIO_15, GPIO_OUTPUT_HIGH);
	gpio_write(GPIOD, GPIO_13, GPIO_OUTPUT_HIGH);
	//printf("Done\n");
	while(1)
	{
		update_dutycycle(1);
		delay_ms(2000);
		for (int i = 1; i<25; i++)
		{

			delay_ms(100);
			update_dutycycle(i);
			#ifdef SEMIHOSTING
			printf("dutycycle: %d\r\n", i);
			#endif
		
		}
		
		delay_ms(2000);
	}

}