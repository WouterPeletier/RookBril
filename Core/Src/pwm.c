#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
//CommentOmTeTesten
#include "platform.h"
#include "gpio.h"

void init_pwm(TIM_TypeDef* TIMx, uint16_t dutycycle, uint32_t frequency, GPIO_TypeDef* Port, uint8_t pin)
{
    if(dutycycle > 50) dutycycle = 50;
    
    uint16_t prescal = 0;
    uint16_t autoreload = (F_CPU/(2*frequency*(prescal+1)))-1;
    uint16_t ccr1 = (dutycycle*autoreload)/100;
    uint16_t ccr2 = autoreload - ccr1;
    
    
    #ifdef SEMIHOSTING
    printf("initialising PWM\r\n");
    printf("Requested values\r\n");
    printf("dutycycle: %d\t frequency: %d\r\n", dutycycle, frequency);
    printf("\nActual\r\n");
    printf("dutycycle1: %d\tdutycycle2: %d\t frequency: %d Hz\r\n", ((ccr1*100)/autoreload), ((ccr2*100)/autoreload), ((F_CPU)/((autoreload+1)*(prescal+1))) );

    printf("Debug: PSC: %d\tARR: %d\tCCR1: %d\tCCR2: %d\r\n", prescal, autoreload, ccr1, ccr2);

    #endif
    if(TIMx == TIM2) {
    	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    } else if(TIMx == TIM3) {
    	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    } else if(TIMx == TIM4) {
    	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    } else if(TIMx == TIM5) {
    	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    }
    TIMx->PSC = prescal;
    TIMx->ARR = autoreload;
    TIMx->CCR1 = ccr1;
    TIMx->CCR2 = ccr2;

    /* center aligned, mode 3: Output compare interrupt flags set when counting up and down */
    TIMx->CR1 |= (3<<TIM_CR1_CMS_Pos);

    /* Output 1 is active when CNT<CCR1 */
    TIMx->CCMR1 |= (0b110<<TIM_CCMR1_OC1M_Pos);
    /* Output 2 is active when CNT>CCR2 */
    TIMx->CCMR1 |= (0b111<<TIM_CCMR1_OC2M_Pos);

    /* enable GPIO outputs */
//    init_gpio(Port, pin, GPIO_MODER_ALT, GPIO_ALTFUNC_2, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);

    init_gpio(GPIOA, GPIO_6, GPIO_MODER_ALT, GPIO_ALTFUNC_2, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);
    init_gpio(GPIOA, GPIO_7, GPIO_MODER_ALT, GPIO_ALTFUNC_2, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);


    /* invert 1 of the timer outputs */
    TIMx->CCER |= TIM_CCER_CC2P;
    TIMx->CCER |= TIM_CCER_CC1P;
    /* Connect timer output to gpio output*/
    TIMx->CCER |= (TIM_CCER_CC2E | TIM_CCER_CC1E);

    TIMx->CR1 ^= TIM_CR1_CEN;
}

void update_dutycycle(TIM_TypeDef* TIMx, uint32_t dutycycle)
{
    uint32_t autoreload = TIMx->ARR;
    uint16_t ccr1 = (dutycycle*autoreload)/100;
    uint16_t ccr2 = autoreload - ccr1;

    
    TIMx->CCR1 = ccr1;
    TIMx->CCR2 = ccr2;
}

void togglePWM(TIM_TypeDef* TIMx) {
	/* Enable timer */
	TIMx->CR1 ^= TIM_CR1_CEN;
}

void switchPWM(TIM_TypeDef* TIMx, uint8_t HL) {
	if(HL == 0) {
		TIMx->CR1 &= ~TIM_CR1_CEN;
	} else if(HL == 1) {
		TIMx->CR1 |= TIM_CR1_CEN;
	}
}
