#include "stm32f4xx.h"
#include "interrupts.h"
#include "gpio.h"
#include "pwm.h"

#define DEBUG

#ifdef DEBUG
  #define DEBUGLOG(...) printf(__VA_ARGS__)
#else
  #define DEBUGLOG(...)
#endif

void receive()
{  
    DEBUGLOG("Receive function called\r\n");
    initTIMIRR(50, 38000);
    set_interrupt(GPIOD, GPIO_0, INT_ALL_EDGE);  //MOET pin 0 zijn voor EXTI 0. Pin nummer = EXTI nummer
}