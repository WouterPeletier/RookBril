#include "interrupts.h"
#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include "gpio.h"

/* TEST THIS */
void set_interrupt(GPIO_TypeDef* port, uint8_t pin, uint8_t edgeRate)
{
    //getest met mode output??
	init_gpio(port, pin, GPIO_MODER_INPUT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);
    //interrupt mask bits EXTI_IMR
    EXTI->IMR |= 1<<pin;
    //trigger selection EXTI_RTSR and EXTI_FTSR
    EXTI->RTSR |= !!(edgeRate & 0b01);  //rising edge
    EXTI->FTSR |= !!(edgeRate & 0b10); //falling edge

    //Configure the enable and mask bits that control the NVIC IRQ channel mapped to the 
    //external interrupt controller (EXTI) so that an interrupt coming from one of the 23 lines
    //can be correctly acknowledged

    NVIC_SetPriority(EXTI0_IRQn, 0);
    NVIC_EnableIRQ(EXTI0_IRQn);

}

void EXTI0_IRQHandler(void)
{
    if(EXTI->PR & (1<<0))
    {
        #ifdef SEMIHOSTING
        printf("Interrupt");
        #endif

        //verbetering
		update_dutycycle(0);
    }
    EXTI->PR |= 1;
    
}
