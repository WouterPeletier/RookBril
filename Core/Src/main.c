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

    receive();

    while(1)
    {
        __WFI();
    }

    // while(1)
    // {
    //     int test = gpio_read(GPIOD, GPIO_0);
    //     printf("Read value: %d", test);
    //     //__WFI();
    // }
    // IRMsg->IRAddress = 1;

    
    // initTIM(TIM3, 25, 1000);
    // initPWM(GPIOA, GPIO_7, TIM3);
    // switchPWM(TIM3, 1);
    // togglePWM(TIM3);
    // update_dutycycle(TIM3, 50);
    // while(1){}
    // printf("Done\n");
    // while(1)
    // {
    //     printf("Running");
    //     update_dutycycle(TIM3, 1);
    //     delay_ms(2000);
    //     for (int i = 1; i<25; i++)
    //     {
    //         delay_ms(100);
    //         update_dutycycle(TIM3, i);
    //     }
    //     int test = gpio_read(GPIOD, GPIO_0);
    //     printf("Read value: %d", test);
    //     delay_ms(2000);
    // }
    
}

void InitIR(uint8_t address, IRMode mode) 
{
    initPWM(GPIOA, GPIO_6, TIM3);
}