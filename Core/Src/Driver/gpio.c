#include <stddef.h>
#include <stdint.h>
#include <stm32f4xx.h>

#include "gpio.h"

void gpio_write(GPIO_TypeDef* port, uint8_t pin, uint8_t level)
{
    // check if pointer is not empty
    if (port == NULL) return;
    // pin number can't be over 16
    if (pin >= 16) return;

    if (level == 0)
    {
        port->BSRR |= (0x01 << (16 + pin));
    }
    else
    {
        port->BSRR |= (0x01 << (pin));
    }
}

void gpio_toggle(GPIO_TypeDef* port, uint8_t pin)
{
	// check if pointer is not empty
	if (port == NULL) return;
	// pin number can't be over 16
	if (pin >= 16) return;

	port->ODR ^= (0x01 << pin);
}

uint8_t gpio_read(GPIO_TypeDef* port, uint8_t pin)
{
    // check if pointer is not empty
    if (port == NULL) return 0;

    return ((port->IDR & (0x01 << pin)) >> pin);
}

void gpio_configure_pin(gpio_opt_t gpio_opt)
{
    uint32_t reg;

    // check if pointer is not empty
    if (gpio_opt.Port == NULL) return;

    // Pin speed
    reg = gpio_opt.Port->OSPEEDR;
    reg &= ~(0x03 << (gpio_opt.Pin * 2));           // clear bits
    reg |= (gpio_opt.Speed << (gpio_opt.Pin * 2));  // write bits
    gpio_opt.Port->OSPEEDR = reg;

    // output type (open-drain / push-pull)
    reg = gpio_opt.Port->OTYPER;
    reg &= ~(0x01 << (gpio_opt.Pin));
    reg |= (gpio_opt.Type << gpio_opt.Pin);
    gpio_opt.Port->OTYPER = reg;

    // Pull up/down
    reg = gpio_opt.Port->PUPDR;
    reg &= ~(0x03 << (gpio_opt.Pin * 2));           // clear bits
    reg |= (gpio_opt.Pull << (gpio_opt.Pin * 2));   // write bits
    gpio_opt.Port->PUPDR = reg;

    // GPIO Mode
    reg = gpio_opt.Port->MODER;
    reg &= ~(0x03 << (gpio_opt.Pin * 2));           // clear bits
    reg |= (gpio_opt.Mode << (gpio_opt.Pin * 2)); // write bits
    gpio_opt.Port->MODER = reg;


    // Alternate function
    if (gpio_opt.Pin < 8) {
        reg = gpio_opt.Port->AFR[0];
        reg &= ~(0x0F << (gpio_opt.Pin * 4));
        reg |= (gpio_opt.Alternate << (gpio_opt.Pin * 4));
        gpio_opt.Port->AFR[0] = reg;
    } else {
        reg = gpio_opt.Port->AFR[1];
        reg &= ~(0x0F << ((gpio_opt.Pin - 8) * 4));
        reg |= (gpio_opt.Alternate << ((gpio_opt.Pin - 8) * 4));
        gpio_opt.Port->AFR[1] = reg;
    }
}

void init_gpio_options(gpio_opt_t options)
{
	if(options.Port == GPIOA) RCC->AHB1ENR |=   RCC_AHB1ENR_GPIOAEN;
	if(options.Port == GPIOB) RCC->AHB1ENR |=   RCC_AHB1ENR_GPIOBEN;
	if(options.Port == GPIOC) RCC->AHB1ENR |=   RCC_AHB1ENR_GPIOCEN;
	if(options.Port == GPIOD) RCC->AHB1ENR |=   RCC_AHB1ENR_GPIODEN;


    gpio_configure_pin(options);
}


void init_gpio(GPIO_TypeDef* port, uint8_t pin, uint8_t mode, uint8_t alternate, uint8_t type, uint8_t pull, uint8_t speed)
{
    gpio_opt_t gpio_opt;

    gpio_opt.Port = port;
    gpio_opt.Pin = pin;
    gpio_opt.Mode = mode;
    gpio_opt.Alternate = alternate;
    gpio_opt.Type = type;
    gpio_opt.Pull = pull;
    gpio_opt.Speed = speed;

    init_gpio_options(gpio_opt);
}
