#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include <stdbool.h>

#include "platform.h"
#include "gpio.h"
#include "pwm.h"
#include "interrupts.h"
#include "IR.h"

#define Address  1 //Address tussen 0 en 32
#define DEBUG

#ifdef SEMIHOSTING
    extern void initialise_monitor_handles(void);
#endif

#ifdef DEBUG
  #define DEBUGLOG(...) printf(__VA_ARGS__)
#else
  #define DEBUGLOG(...)
#endif

IRMode IRSendReceive = Send;
IRPacket * IRMsg = {0};
uint32_t bitCycles = 4800; //De tijd van de bit in clock cycles = 600us/(1/F_CPU (8000000))
uint32_t sendIntervalms = 1000; //De tijd in ms hoelang het duurt tot de volgende IR send command

uint16_t receivedIR;
bool receiveFlag = false;

int main(void)
{
    #ifdef SEMIHOSTING
        initialise_monitor_handles();
    #endif

    init_platform();
    DEBUGLOG("Platform Initiated\r\n");
    GPIOD->MODER |= GPIO_MODER_MODER11_0 | GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0;
    GPIOD->ODR |= GPIO_ODR_OD12 | GPIO_ODR_OD11;

    receive();

    while(1)
    {
        __WFI();
    }
}

void InitIR(uint8_t address, IRMode mode) 
{
    initPWM(GPIOA, GPIO_6, TIM3);
}