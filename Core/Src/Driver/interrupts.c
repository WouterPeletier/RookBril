#include "interrupts.h"
#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include "gpio.h"
#include "pwm.h"
#include <stdbool.h>

#define messageLength 12
#define DEBUG

#ifdef DEBUG
  #define DEBUGLOG(...) printf(__VA_ARGS__)
#else
  #define DEBUGLOG(...)
#endif

bool start = true;
uint8_t bitIndex = 0;
uint16_t lowDuration;
uint8_t message[messageLength];

extern uint16_t receivedIR;
extern bool receiveFlag;

void set_interrupt(GPIO_TypeDef* port, uint8_t pin, uint8_t edgeRate)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    EXTI->IMR |= 1<<pin;
    //getest met mode output??
	init_gpio(port, pin, GPIO_MODER_INPUT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);

    //trigger selection EXTI_RTSR and EXTI_FTSR
    EXTI->RTSR |= !!(edgeRate & 0b01);  //rising edge
    EXTI->FTSR |= !!(edgeRate & 0b10); //falling 
    
    // SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PD;
    if(port == GPIOA) // Only works for pin 0
    {
        SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
        DEBUGLOG("EXTI line 0 set to use port A\r\n");
    }else if(port == GPIOB)
    {
        SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB;
        DEBUGLOG("EXTI line 0 set to use port B\r\n");
    }else if(port == GPIOC)
    {
        SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC;
        DEBUGLOG("EXTI line 0 set to use port C\r\n");
    }else if(port == GPIOD)
    {
        SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PD;
        DEBUGLOG("EXTI line 0 set to use port D\r\n");
    }

    EXTI->PR |= EXTI_PR_PR0;

    NVIC_SetPriority(EXTI0_IRQn, 0);

    //Enable interrupt in NVIC
    NVIC_EnableIRQ(EXTI0_IRQn);
    //Enable interrupt in EXTI module
    
    DEBUGLOG("Interrupt set\r\n");
    //EXTI->PR |= EXTI_PR_PR0; // Clear the EXTI line 0 pending flag

}

void EXTI0_IRQHandler(void)
{
    //DEBUGLOG("HANDLER\r\n");
    if(start)
    {
        if (gpio_read(GPIOD, GPIO_0) == 0)
        {
            //DEBUGLOG("Detected falling edge\r\n");
            TIM4->CNT = 0;  //Make sure CNT register is 0
            switchPWM(TIM4, 1); //Start timer TIM4
            start = false;  //Set flag to false
        }  
    }else
    {
        //DEBUGLOG("Detected rising edge\r\n");
        switchPWM(TIM4, 0); //Stop timer
        lowDuration = TIM4->CNT; //Capture lowDuration

        TIM4->DIER |= TIM_DIER_UIE;  // Enable TIM4 update interrupt
        TIM4->ARR = lowDuration * 1.5;  //Auto reload is 3/4 bitTime
        //NVIC_DisableIRQ(EXTI0_IRQn);  //Disable this interrupt
        EXTI->IMR &= 0<<0; //Disable this interrupt
        start = true;  //Reset start flag
        NVIC_EnableIRQ(TIM4_IRQn);
        TIM4->CNT = 0;
        switchPWM(TIM4, 1); //Start timer TIM4
    }

    EXTI->PR |= EXTI_PR_PR0; // Clear the EXTI line 0 pending flag
}

void TIM4_IRQHandler(void) 
{
    //DEBUGLOG("Got to TIM4 interrupt\r\n");
    TIM4->SR &= ~TIM_SR_UIF;
    TIM4->ARR = lowDuration * 2;
    message[bitIndex] = gpio_read(GPIOD, GPIO_0);
    bitIndex++;
    if(bitIndex == messageLength)
    {
        bitIndex = 0;
        TIM4->DIER &= TIM_DIER_UIE;  //Disable this interrupt
        EXTI->IMR |= 1<<0; //Enable EXTI interrupt
        for(int i = 0; i<messageLength; i++)
        {
            receivedIR << 1;
            receivedIR |= message[i];
        }
        receiveFlag = true;
        switchPWM(TIM4, 0); //Stop timer
    }
}