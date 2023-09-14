#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include "platform.h"
#include "gpio.h"

/* Semihosting */
extern void initialise_monitor_handles(void);


int main(void)
{
	initialise_monitor_handles();	/* init semihosting */

	init_platform();
	init_gpio(GPIOD, GPIO_15, GPIO_MODER_OUTPUT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_LOW);
	gpio_write(GPIOD, GPIO_15, GPIO_OUTPUT_HIGH);
	printf("Done\n");
	while(1)
	{
		delay_ms(1000);
		gpio_toggle(GPIOD, GPIO_15);
		printf("Hoid\n");

	}

}
