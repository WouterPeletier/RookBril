#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdint.h>
#include <stm32f4xx.h>

/* Delay function, works by using SysTicks, the first ms is not precise */
void delay_ms(uint32_t delay);

/* Initialise the clock and SysTicks*/
void init_platform(void);

uint64_t get_sysTicks(void);	/* Get current systicks (uptime) */

/* Define pins */



#endif
