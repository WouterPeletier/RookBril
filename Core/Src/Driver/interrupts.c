#include "interrupts.h"
#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include "gpio.h"
#include "pwm.h"
#include <stdbool.h>

#define messageLength 13
#define DEBUG

#ifdef DEBUG
 #define DEBUGLOG(...) //printf(__VA_ARGS__)
#else
 #define DEBUGLOG(...)
#endif

bool start = true; //Start of message flag
bool bitStart = true; //Flag used for bit control
bool control; 
bool invalidFlag = false; //Flag used for message control
uint8_t bitIndex = 0;
uint16_t lowDuration;
uint8_t message[messageLength];
uint8_t msgPart = 1;
uint8_t msgFlag = 0;
uint8_t oldPD0 = 0;

extern uint16_t receivedIR;
extern bool receiveFlag;

int validIDs[] = {0b0000, 0b0001, 0b0010, 0b0011, 0b0100}; //Define valid messages IDs
int validMessages[] = {0b0000, 0b0001, 0b0010, 0b0011, 0b0100}; //Define valid messages content

void set_interrupt(GPIO_TypeDef* port, uint8_t pin, uint8_t edgeRate)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    init_gpio(port, pin, GPIO_MODER_INPUT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);
    GPIOD->MODER &= ~(GPIO_MODER_MODE0);  // Clear bits
    GPIOD->PUPDR &= ~(GPIO_PUPDR_PUPD0);  // No pull-up, no pull-down

    SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0);  // Clear EXTI0 selection
    
    //Port selection
    if(port == GPIOA) // Only works for pin 0
    {
        SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
    //DEBUGLOG("EXTI line 0 set to use port A\r\n");
    }else if(port == GPIOB)
    {
        SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PB;
        DEBUGLOG("EXTI line 0 set to use port B\r\n");
    }else if(port == GPIOC)
    {
        SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC;
    //DEBUGLOG("EXTI line 0 set to use port C\r\n");
    }else if(port == GPIOD)
    {
        SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PD;
    //DEBUGLOG("EXTI line 0 set to use port D\r\n");
    }

    EXTI->IMR |= 1<<pin;

    //trigger selection EXTI_RTSR and EXTI_FTSR
    EXTI->RTSR |= (!!(edgeRate & 0b01))<<pin;  //rising edge
    EXTI->FTSR |= (!!(edgeRate & 0b10))<<pin; //falling

    EXTI->PR |= EXTI_PR_PR2;

    NVIC_SetPriority(EXTI2_IRQn, 0);

    //Enable interrupt in NVIC
    NVIC_EnableIRQ(EXTI2_IRQn);
    //Enable interrupt in EXTI module
    
    //DEBUGLOG("Interrupt set\r\n");
    //EXTI->PR |= EXTI_PR_PR0; // Clear the EXTI line 0 pending flag
}

void EXTI2_IRQHandler(void)
{
    //Output pins used for debugging
    // if(gpio_read(GPIOB, GPIO_2) == 0) 
    // {
    //     GPIOD->ODR &= ~GPIO_ODR_OD12;
    // } else {
    //     GPIOD->ODR |= GPIO_ODR_OD12;
    // }

    //Output pins used for debugging
	if(gpio_read(GPIOB, GPIO_2)== 1 )
	{
	 GPIOD->ODR &= ~GPIO_ODR_OD11;
	} else {
	 GPIOD->ODR |= GPIO_ODR_OD11;
	}
    GPIOD->ODR |= GPIO_ODR_OD13;
    //GPIOD->ODR ^= GPIO_ODR_OD12;
    if(start)
    {
        if (!gpio_read(GPIOB, GPIO_2))//Check for falling edge
        {
            //GPIOD->ODR |= GPIO_ODR_OD14;
            TIM4->CNT = 0;  //Make sure CNT register is 0
            switchPWM(TIM4, 1); //Start timer TIM4
            start = false;  //Set flag to false
        }
    }else {
        //DEBUGLOG("Detected rising edge\r\n");
        switchPWM(TIM4, 0); //Stop timer
        lowDuration = TIM4->CNT; //Capture lowDuration 7112 = 889us, 4802 = 600 us
        TIM4->CNT = 0;
        /* LowDuration check uitgecomment voor testen*/
        // lowDuration = 6480;
        // if(lowDuration < 4000) 
        // {
        //     TIM4->CNT = 0;
        //     switchPWM(TIM4, 1); //Start timer
        //     EXTI->PR |= EXTI_PR_PR2;
        // } else if(lowDuration > 10000) {
        //     TIM4->CNT = 0;
        //     switchPWM(TIM4, 1); //Start timer
        //     EXTI->PR |= EXTI_PR_PR2;
        // } else {
		if(lowDuration >= 6000 && lowDuration <= 8100) { //lowDuration tussen 815us en 950us
			lowDuration = 7112; //lowDuration is 889us
		} else {
			lowDuration = 7112;
		}
		//     lowDuration = 6480;
		TIM4->DIER |= TIM_DIER_UIE;  // Enable TIM4 update interrupt
		TIM4->ARR = lowDuration * 1.1;  //Auto reload is 3/4 bitTime
		//NVIC_DisableIRQ(EXTI0_IRQn);  //Disable this interrupt
		EXTI->IMR &= 0<<0; //Disable this interrupt
		NVIC_DisableIRQ(EXTI2_IRQn);
		start = true;  //Reset start flag
		//GPIOD->ODR &= ~GPIO_ODR_OD14;
		NVIC_EnableIRQ(TIM4_IRQn);
		switchPWM(TIM4, 1); //Start timer TIM4
        // }
    }

    EXTI->PR |= EXTI_PR_PR2; // Clear the EXTI line 2 pending flag
}

int validID(int received)
{
    // Iterate through the valid messages and check for a match
    for (int i = 0; i < sizeof(validIDs) / sizeof(validIDs[0]); i++) 
    {
        if (received == validIDs[i]) 
        {
            DEBUGLOG("Valid ID");
            return 1; //Valid message
        }
    }
    DEBUGLOG("Invalid ID");
    return 0; //Invalid message
}

int validMessage(int received)
{
    // Iterate through the valid messages and check for a match
    for (int i = 0; i < sizeof(validMessages) / sizeof(validMessages[0]); i++) 
    {
        if (received == validMessages[i]) 
        {
            DEBUGLOG("Valid message");
            return 1; //Valid message
        }
    }
    DEBUGLOG("Invalid message");
    return 0; //Invalid message
}

void reset()
{
    invalidFlag = false;
    receivedIR = 0;
    switchPWM(TIM4, 0); //Stop timer
    GPIOD->ODR &= ~GPIO_ODR_OD13;
    EXTI->IMR |= 1<<2; //Enable EXTI interrupt
    NVIC_EnableIRQ(EXTI2_IRQn);
    TIM4->ARR = 65000;
    TIM4->DIER &= TIM_DIER_UIE;  //Disable timer interrupt
}

void TIM4_IRQHandler(void) 
{
    //DEBUGLOG("Got to TIM4 interrupt\r\n");
    //GPIOD->ODR |= GPIO_ODR_OD11;
	if(bitIndex > messageLength) {
		bitIndex = 0;
	}
    GPIOD->ODR ^= GPIO_ODR_OD14;
	if(gpio_read(GPIOB, GPIO_2)== 1 )
	{
		GPIOD->ODR &= ~GPIO_ODR_OD11;
	} else {
		GPIOD->ODR |= GPIO_ODR_OD11;
	}

    if(bitStart)
    {
        TIM4->ARR = lowDuration;// * 2; //Was 1*bit time, now 0.5 to control bits
        control = gpio_read(GPIOB, GPIO_2);
        bitStart = false;
    }else{

        if(control != gpio_read(GPIOB, GPIO_2))//Check if bit is valid
        {
            GPIOD->ODR ^= GPIO_ODR_OD12;
            message[bitIndex] = gpio_read(GPIOB, GPIO_2); //Invert data
        }else{
            invalidFlag = true;
        }
        bitIndex++;
        bitStart = true;
        if(bitIndex >= messageLength)
        {
            if(!invalidFlag)
            {
//            	GPIOD->ODR ^= GPIO_ODR_OD9;
            	invalidFlag = false;
				receivedIR = 0;
				bitIndex = 0;
				switchPWM(TIM4, 0); //Stop timer
				GPIOD->ODR &= ~GPIO_ODR_OD13;
				EXTI->IMR |= 1<<2; //Enable EXTI interrupt
				NVIC_EnableIRQ(EXTI2_IRQn);
				TIM4->CNT = 0;
				TIM4->ARR = 65000;
				TIM4->DIER &= TIM_DIER_UIE;  //Disable timer interrupt
                return;
            }
            bitIndex = 0;
            for(int i = 0; i<messageLength; i++)
            {
                receivedIR = receivedIR << 1;
                receivedIR |= message[i];
                uint8_t x = message[i];
            }
            receiveFlag = true;

            unsigned int receivedID = (receivedIR>>1 & 0b11110000) >> 4; //extract bit 7-4
            unsigned int receivedMessage = (receivedIR>>1 & 0b1111); //Extract last 4 bits
            if((receivedID == 8) && (receivedMessage == 3)) {
            	GPIOD->ODR |= GPIO_ODR_OD9;
            } else {
            	GPIOD->ODR &= ~(GPIO_ODR_OD9);
            }
            invalidFlag = false;
			receivedIR = 0;
			bitIndex = 0;
			switchPWM(TIM4, 0); //Stop timer
			GPIOD->ODR &= ~GPIO_ODR_OD13;
			EXTI->IMR |= 1<<2; //Enable EXTI interrupt
			NVIC_EnableIRQ(EXTI2_IRQn);
			TIM4->CNT = 0;
			TIM4->ARR = 65000;
			TIM4->DIER &= TIM_DIER_UIE;  //Disable timer interrupt
        }
    }
    //GPIOD->ODR &= ~GPIO_ODR_OD11;
    TIM4->SR &= ~TIM_SR_UIF;
}
