#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>

#include "platform.h"
#include "gpio.h"

void init_pwm(uint16_t dutycycle, uint32_t frequency)
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
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = prescal;
    TIM3->ARR = autoreload;
    TIM3->CCR1 = ccr1;
    TIM3->CCR2 = ccr2;
    
    /* center aligned, mode 3: Output compare interrupt flags set when counting up and down */
    TIM3->CR1 |= (3<<TIM_CR1_CMS_Pos); 

    //CCR1:     2 CCR2: 4 ARR:6
    //CNT:      0 1 2 3 4 5 6 5 4 3 2 1 0 1 2 3 4 5 6 5
    //OC1:      1 1 0 0 0 0 0 0 0 0 0 1 1 1 0 0 0 0 0 0   
    //OC2:      0 0 0 0 0 1 1 1 0 0 0 0 0 0 0 0 0 1 1 1
    //
    //This creates a sine wave

    /* Output 1 is active when CNT<CCR1 */
    TIM3->CCMR1 |= (0b110<<TIM_CCMR1_OC1M_Pos);
    /* Output 2 is active when CNT>CCR2 */
    TIM3->CCMR1 |= (0b111<<TIM_CCMR1_OC2M_Pos);

    /* Tim3_CH1 PA6 */
    /* Tim3_CH2 PA7 */

    /* enable GPIO outputs */
    init_gpio(GPIOA, GPIO_6, GPIO_MODER_ALT, GPIO_ALTFUNC_2, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);
    init_gpio(GPIOA, GPIO_7, GPIO_MODER_ALT, GPIO_ALTFUNC_2, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);

    /* invert 1 of the timer outputs */
    TIM3->CCER |= TIM_CCER_CC2P;
    TIM3->CCER |= TIM_CCER_CC1P;
    /* Connect timer output to gpio output*/
    TIM3->CCER |= (TIM_CCER_CC2E | TIM_CCER_CC1E);
    /* Enable timer */
    TIM3->CR1 |= TIM_CR1_CEN;

}

void update_dutycycle(uint32_t dutycycle)
{
    uint32_t autoreload = TIM3->ARR;
    uint16_t ccr1 = (dutycycle*autoreload)/100;
    uint16_t ccr2 = autoreload - ccr1;

    
    TIM3->CCR1 = ccr1;
    TIM3->CCR2 = ccr2;
}