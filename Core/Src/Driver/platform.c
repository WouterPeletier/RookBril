#include "platform.h"
#include <stdint.h>
#include <stm32f4xx.h>

/* Global variables */
static volatile uint64_t gSysticks;


/* Function Prototypes */
static void init_clock(void);
static void init_systick(void);


void init_platform(void)
{
	init_clock();
	init_systick();
}

void delay_ms(uint32_t delay)
{
	uint64_t prev, now;
	prev = now = get_sysTicks();		/* Get current sysTickTime */

	while ((now - prev) < delay) {
    	__WFI();
		now = get_sysTicks();			/* Wait for interrupt, and check if enough time has passed */
	}
}

uint64_t get_sysTicks(void)
{
	return gSysticks;
}


static void init_clock(void)
{
	/* Initialise clock to something reasonable */
}

static void init_systick(void)
{
	SysTick_Config(F_CPU / 1000);		// every ms
}
/* Systick handler runs every ms */
void SysTick_Handler(void)
{
	gSysticks++;
}
