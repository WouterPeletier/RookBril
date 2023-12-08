 #include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include <stdbool.h>

#include "platform.h"
#include "gpio.h"
#include "pwm.h"
#include "interrupts.h"
#include "user_interface.h"
#include "IR.h"
#include "fonts.h"
#include "ssd1306.h"

#ifdef SEMIHOSTING
   extern void initialise_monitor_handles(void);
#endif

#ifdef DEBUG
 #define DEBUGLOG(...) printf(__VA_ARGS__)
#else
 #define DEBUGLOG(...)
#endif


#define Receiving
int32_t Address = 1; //Address tussen 0 en 32
int32_t PDLC_intensity = 0;

IRMode IRSendReceive = Send;
IRPacket * IRMsg = {0};
uint32_t bitCycles = 4800; //De tijd van de bit in clock cycles = 600us/(1/F_CPU (8000000))
uint32_t sendIntervalms = 1000; //De tijd in ms hoelang het duurt tot de volgende IR send command


uint16_t receivedIR;
bool receiveFlag = false;
bool sendFlag = false;

void beacon_main(void);
void bril_main(void);

int main(void){

    #ifdef SEMIHOSTING
        initialise_monitor_handles();
    #endif  

  
    init_platform();
    //DEBUGLOG("Platform Initiated\r\n");

    beacon_main();
}

void bril_main(void)
{
    DEBUGLOG("Running Bril Code");
	init_gpio(GPIOB, GPIO_2, GPIO_MODER_INPUT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);

    RCC->AHB1ENR = RCC_AHB1ENR_GPIODEN;
    // Set all LEDs output
    GPIOD->MODER |= GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER9_0;

    // Turn all LEDs Off
    GPIOD->ODR &= ~(GPIO_ODR_OD12 | GPIO_ODR_OD14 | GPIO_ODR_OD11 | GPIO_ODR_OD13 | GPIO_ODR_OD9);
    init_gpio(GPIOA, GPIO_4, GPIO_MODER_OUTPUT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);
    gpio_write(GPIOA, GPIO_4, 1);
    init_gpio(GPIOA, GPIO_0, GPIO_MODER_OUTPUT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);
    gpio_write(GPIOA, GPIO_0, 1);
    initTimPDLC2(50, 100);
    receive();

    // uint8_t PD13 = 0;
    while(1)
    {
        __WFI();

    }
}

void beacon_main(void) {
/*	DEBUGLOG("Running Beacon Code");
	IRMsg->IRAddress = 1;

	DEBUGLOG("initializing IR LED GPIO");
	init_gpio(GPIOA, GPIO_0, GPIO_MODER_ALT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_NONE, GPIO_OSPEEDR_HIGH);*/

	/*
	DEBUGLOG("initializing encoder GPIO's");
	init_gpio(GPIOA, GPIO_0, GPIO_MODER_INPUT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_DOWN, GPIO_OSPEEDR_LOW);
	set_interrupt(GPIOA, GPIO_0, INT_RISING_EDGE);

	init_gpio(GPIOA, GPIO_0, GPIO_MODER_INPUT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_DOWN, GPIO_OSPEEDR_LOW);
	set_interrupt(GPIOA, GPIO_0, INT_RISING_EDGE);

	init_gpio(GPIOA, GPIO_0, GPIO_MODER_INPUT, GPIO_ALTFUNC_0, GPIO_OTYPER_PUSHPULL, GPIO_PULL_DOWN, GPIO_OSPEEDR_LOW);
	set_interrupt(GPIOA, GPIO_0, INT_RISING_EDGE);
	*/


	//DEBUGLOG("initializing I2C GPIO's");
	//init_gpio(GPIOA, GPIO_13, GPIO_MODER_ALT, GPIO_ALTFUNC_4, GPIO_OTYPER_OPENDRAIN, GPIO_PULL_UP, GPIO_OSPEEDR_HIGH);
	//init_gpio(GPIOA, GPIO_14, GPIO_MODER_ALT, GPIO_ALTFUNC_4, GPIO_OTYPER_OPENDRAIN, GPIO_PULL_UP, GPIO_OSPEEDR_HIGH);



	//DEBUGLOG("initializing I2C and display driver");
	RCC->AHB1ENR |= (1<<1); //| (1<<21);

	GPIOB->MODER |= (2<<12) | (2<<14);  // Bits (17:16)= 1:0 --> Alternate Function for Pin PB8; Bits (19:18)= 1:0 --> Alternate Function for Pin PB9

	GPIOB->OTYPER |= (1<<GPIO_OTYPER_OT6_Pos) | (1<<GPIO_OTYPER_OT7_Pos);  //  Bit8=1, Bit9=1  output open drain

	GPIOB->OSPEEDR |= (3<<GPIO_OSPEEDR_OSPEED6_Pos) | (3<<GPIO_OSPEEDR_OSPEED7_Pos);  // Bits (17:16)= 1:1 --> High Speed for PIN PB8; Bits (19:18)= 1:1 --> High Speed for PIN PB9

	GPIOB->PUPDR |= (1<<GPIO_PUPDR_PUPD6_Pos) | (1<<GPIO_PUPDR_PUPD7_Pos);  // Bits (17:16)= 0:1 --> Pull up for PIN PB8; Bits (19:18)= 0:1 --> pull up for PIN PB9

	GPIOB->AFR[0] |= (4<<GPIO_AFRL_AFSEL7_Pos) | (4<<GPIO_AFRL_AFSEL6_Pos);  // Bits (3:2:1:0) = 0:1:0:0 --> AF4 for pin PB8;  Bits (7:6:5:4) = 0:1:0:0 --> AF4 for pin PB9

	SSD1306_Init();

	init_settings();

/*	gpio_toggle(GPIOA, GPIO_0);
	initTIMIRS(50, 38000);
	IRInit(0b00110, Send);
	for(volatile uint32_t j = 0; j<400000; j++) {
	}
	IRSend(0b011010);*/

	while(1){
/*		for (int i = 0; i<settings_count; ++i)
		{
			display_menu(i);
			for (int j = 0; j < 1000000; j++);

			display_setting(i);
			for (int j = 0; j < 1500000; j++);

			display_menu(i);
			for (int j = 0; j < 1000000; j++);
		}*/
		iterate_UI(CW);
		for (int j = 0; j < 500000; j++);
		iterate_UI(CW);
		for (int j = 0; j < 500000; j++);
		iterate_UI(CCW);
		for (int j = 0; j < 500000; j++);

		iterate_UI(PB);
		for (int j = 0; j < 500000; j++);

	}
}
