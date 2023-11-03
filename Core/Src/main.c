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
    init_gpio(GPIOB, GPIO_2, GPIO_MODER_INPUT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);


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