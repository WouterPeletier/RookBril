#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include <stdbool.h>

#include "platform.h"
#include "gpio.h"
#include "pwm.h"
#include "interrupts.h"
#include "IR.h"

#include "fonts.h"
#include "ssd1306.h"
#include "user_interface.h"

#define Address  1 //Address tussen 0 en 32

#ifdef SEMIHOSTING
   extern void initialise_monitor_handles(void);
#endif

#ifdef DEBUG
 #define DEBUGLOG(...) //printf(__VA_ARGS__)
#else
 #define DEBUGLOG(...)
#endif

IRMode IRSendReceive = Send;
IRPacket * IRMsg = {0};
uint32_t bitCycles = 4800; //Bit time in in clock cycles = 600us/(1/F_CPU (8000000))
uint32_t sendIntervalms = 1000; //Milliseconds delay between IR commands 

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

    RCC->AHB1ENR = RCC_AHB1ENR_GPIODEN;
    // Set all LEDs output
    GPIOD->MODER |= GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER9_0;

    // Turn all LEDs Off
    GPIOD->ODR &= ~(GPIO_ODR_OD12 | GPIO_ODR_OD14 | GPIO_ODR_OD11 | GPIO_ODR_OD13 | GPIO_ODR_OD9);


	//DEBUGLOG("initializing I2C GPIO's");
	init_gpio(GPIOB, GPIO_6, GPIO_MODER_ALT, GPIO_ALTFUNC_4, GPIO_OTYPER_OPENDRAIN, GPIO_PULL_UP, GPIO_OSPEEDR_HIGH); // scl
	init_gpio(GPIOB, GPIO_7, GPIO_MODER_ALT, GPIO_ALTFUNC_4, GPIO_OTYPER_OPENDRAIN, GPIO_PULL_UP, GPIO_OSPEEDR_HIGH); // sda

	//DEBUGLOG("initializing I2C and display driver");
	SSD1306_Init();
	init_settings();

    receive();
    // uint8_t PD13 = 0;
    while(1)
    {
        __WFI();
        unsigned int receivedID = (receivedIR>>1 & 0b11110000) >> 4; //extract bit 7-4
        unsigned int receivedMessage = (receivedIR>>1 & 0b1111); //Extract last 4 bits

    	/* Clear screen */
    	SSD1306_Fill(SSD1306_COLOR_BLACK); // make the screen black

    	char temp_string[9];

    	char ID_string[9];
    	strcpy (ID_string, "ID = ");
    	itoa(receivedID, temp_string, 10);
    	strcat(ID_string, temp_string);

    	char message_string[9];
    	strcpy (message_string, "msg = ");
    	itoa(receivedMessage, temp_string, 10);
    	strcat(message_string, temp_string);

    	SSD1306_GotoXY (0, 28);
    	SSD1306_Puts (ID_string, &Font_16x26, 1);

    	SSD1306_GotoXY (0, 99);
    	SSD1306_Puts (ID_string, &Font_16x26, 1);

    	/* Update screen */
    	SSD1306_UpdateScreen(); // actually display the black screen


    }
}
